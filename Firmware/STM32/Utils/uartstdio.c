//*****************************************************************************
//
// uartstdio.c - Utility driver to provide simple UART console functions.
//

//*****************************************************************************

#include "uartstdio.h"


#include <stdarg.h>
#include "main.h"

//*****************************************************************************
//
//! \addtogroup uartstdio_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If buffered mode is defined, set aside RX and TX buffers and read/write
// pointers to control them.
//

//*****************************************************************************


//*****************************************************************************
//
// This global controls whether or not we are echoing characters back to the
// transmitter.  By default, echo is enabled but if using this module as a
// convenient method of implementing a buffered serial interface over which
// you will be running an application protocol, you are likely to want to
// disable echo by calling UARTEchoSet(false).
//
//*****************************************************************************
//static bool g_bDisableEcho;
static struct _uartstdio_device
	{
		USART_TypeDef * uart;
		IRQn_Type		uart_irqn;
		bool			g_bDisableEcho;
	}uartstdio_device;
//*****************************************************************************
//
// Output ring buffer.  Buffer is full if g_ui32UARTTxReadIndex is one ahead of
// g_ui32UARTTxWriteIndex.  Buffer is empty if the two indices are the same.
//
//*****************************************************************************
static unsigned char g_pcUARTTxBuffer[UART_TX_BUFFER_SIZE];
static volatile uint32_t g_ui32UARTTxWriteIndex = 0;
static volatile uint32_t g_ui32UARTTxReadIndex = 0;

//*****************************************************************************
//
// Input ring buffer.  Buffer is full if g_ui32UARTTxReadIndex is one ahead of
// g_ui32UARTTxWriteIndex.  Buffer is empty if the two indices are the same.
//
//*****************************************************************************
static unsigned char g_pcUARTRxBuffer[UART_RX_BUFFER_SIZE];
static volatile uint32_t g_ui32UARTRxWriteIndex = 0;
static volatile uint32_t g_ui32UARTRxReadIndex = 0;

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_USED          (GetBufferCount(&g_ui32UARTTxReadIndex,  \
                                                &g_ui32UARTTxWriteIndex, \
                                                UART_TX_BUFFER_SIZE))
#define TX_BUFFER_FREE          (UART_TX_BUFFER_SIZE - TX_BUFFER_USED)
#define TX_BUFFER_EMPTY         (IsBufferEmpty(&g_ui32UARTTxReadIndex,   \
                                               &g_ui32UARTTxWriteIndex))
#define TX_BUFFER_FULL          (IsBufferFull(&g_ui32UARTTxReadIndex,  \
                                              &g_ui32UARTTxWriteIndex, \
                                              UART_TX_BUFFER_SIZE))
#define ADVANCE_TX_BUFFER_INDEX(Index) \
                                (Index) = ((Index) + 1) % UART_TX_BUFFER_SIZE

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_USED          (GetBufferCount(&g_ui32UARTRxReadIndex,  \
                                                &g_ui32UARTRxWriteIndex, \
                                                UART_RX_BUFFER_SIZE))
#define RX_BUFFER_FREE          (UART_RX_BUFFER_SIZE - RX_BUFFER_USED)
#define RX_BUFFER_EMPTY         (IsBufferEmpty(&g_ui32UARTRxReadIndex,   \
                                               &g_ui32UARTRxWriteIndex))
#define RX_BUFFER_FULL          (IsBufferFull(&g_ui32UARTRxReadIndex,  \
                                              &g_ui32UARTRxWriteIndex, \
                                              UART_RX_BUFFER_SIZE))
#define ADVANCE_RX_BUFFER_INDEX(Index) \
                                (Index) = ((Index) + 1) % UART_RX_BUFFER_SIZE




//*****************************************************************************
//
// A mapping from an integer between 0 and 15 to its ASCII character
// equivalent.
//
//*****************************************************************************
static const char * const g_pcHex = "0123456789abcdef";

//*****************************************************************************
//
// The list of possible base addresses for the console UART.
//
//*****************************************************************************





//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is full or not.
//!
//! \param pui32Read points to the read index for the buffer.
//! \param pui32Write points to the write index for the buffer.
//! \param ui32Size is the size of the buffer in bytes.
//!
//! This function is used to determine whether or not a given ring buffer is
//! full.  The structure of the code is specifically to ensure that we do not
//! see warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is full or \b false otherwise.
//
//*****************************************************************************

static bool
IsBufferFull(volatile uint32_t *pui32Read,
             volatile uint32_t *pui32Write, uint32_t ui32Size)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    ui32Write = *pui32Write;
    ui32Read = *pui32Read;

    return((((ui32Write + 1) % ui32Size) == ui32Read) ? true : false);
}


//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is empty or not.
//!
//! \param pui32Read points to the read index for the buffer.
//! \param pui32Write points to the write index for the buffer.
//!
//! This function is used to determine whether or not a given ring buffer is
//! empty.  The structure of the code is specifically to ensure that we do not
//! see warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is empty or \b false otherwise.
//
//*****************************************************************************

static bool
IsBufferEmpty(volatile uint32_t *pui32Read,
              volatile uint32_t *pui32Write)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    ui32Write = *pui32Write;
    ui32Read = *pui32Read;

    return((ui32Write == ui32Read) ? true : false);
}


//*****************************************************************************
//
//! Determines the number of bytes of data contained in a ring buffer.
//!
//! \param pui32Read points to the read index for the buffer.
//! \param pui32Write points to the write index for the buffer.
//! \param ui32Size is the size of the buffer in bytes.
//!
//! This function is used to determine how many bytes of data a given ring
//! buffer currently contains.  The structure of the code is specifically to
//! ensure that we do not see warnings from the compiler related to the order
//! of volatile accesses being undefined.
//!
//! \return Returns the number of bytes of data currently in the buffer.
//
//*****************************************************************************

static uint32_t
GetBufferCount(volatile uint32_t *pui32Read,
               volatile uint32_t *pui32Write, uint32_t ui32Size)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    ui32Write = *pui32Write;
    ui32Read = *pui32Read;

    return((ui32Write >= ui32Read) ? (ui32Write - ui32Read) :
           (ui32Size - (ui32Read - ui32Write)));
}


//*****************************************************************************
//
// Take as many bytes from the transmit buffer as we have space for and move
// them into the UART transmit FIFO.
//
//*****************************************************************************

static void
UARTPrimeTransmit()
{

    //
    // Do we have any data to transmit?
    //
    if(!TX_BUFFER_EMPTY)
    {
        //
        // Disable the UART interrupt.  If we don't do this there is a race
        // condition which can cause the read index to be corrupted.
        //
    	NVIC_DisableIRQ(uartstdio_device.uart_irqn);


        //
        // Yes - take some characters out of the transmit buffer and feed
        // them to the UART transmit FIFO.
        //


            LL_USART_TransmitData8(uartstdio_device.uart,
                                      g_pcUARTTxBuffer[g_ui32UARTTxReadIndex]);
            ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxReadIndex);


        //
        // Reenable the UART interrupt.
        //
            NVIC_EnableIRQ(uartstdio_device.uart_irqn);
    }
}


//*****************************************************************************
//
//! Configures the UART console.
//!
//! \param ui32PortNum is the number of UART port to use for the serial console
//! (0-2)
//! \param ui32Baud is the bit rate that the UART is to be configured to use.
//! \param ui32SrcClock is the frequency of the source clock for the UART
//! module.
//!
//! This function will configure the specified serial port to be used as a
//! serial console.  The serial parameters are set to the baud rate
//! specified by the \e ui32Baud parameter and use 8 bit, no parity, and 1 stop
//! bit.
//!
//! This function must be called prior to using any of the other UART console
//! functions: UARTprintf() or UARTgets().  This function assumes that the
//! caller has previously configured the relevant UART pins for operation as a
//! UART rather than as GPIOs.
//!
//! \return None.
//
//*****************************************************************************
void UARTStdioConfig(USART_TypeDef * uart, IRQn_Type uart_irqn, bool enable_echo)
{

    //
    // Flush both the buffers.
    //
    UARTFlushRx();
    UARTFlushTx(true);

    //
    // Remember which interrupt we are dealing with.
    //
    uartstdio_device.uart = uart;
    uartstdio_device.uart_irqn = uart_irqn;
    uartstdio_device.g_bDisableEcho = !enable_echo;
    //
    // We are configured for buffered output so enable the master interrupt
    // for this UART and the receive interrupts.  We don't actually enable the
    // transmit interrupt in the UART itself until some data has been placed
    // in the transmit buffer.
    //
    LL_USART_EnableIT_RXNE(uart);

}

//*****************************************************************************
//
//! Writes a string of characters to the UART output.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//! \param ui32Len is the length of the string to transmit.
//!
//! This function will transmit the string to the UART output.  The number of
//! characters transmitted is determined by the \e ui32Len parameter.  This
//! function does no interpretation or translation of any characters.  Since
//! the output is sent to a UART, any LF (/n) characters encountered will be
//! replaced with a CRLF pair.
//!
//! Besides using the \e ui32Len parameter to stop transmitting the string, if
//! a null character (0) is encountered, then no more characters will be
//! transmitted and the function will return.
//!
//! In non-buffered mode, this function is blocking and will not return until
//! all the characters have been written to the output FIFO.  In buffered mode,
//! the characters are written to the UART transmit buffer and the call returns
//! immediately.  If insufficient space remains in the transmit buffer,
//! additional characters are discarded.
//!
//! \return Returns the count of characters written.
//
//*****************************************************************************
int
UARTwrite(const char *pcBuf, uint32_t ui32Len)
{

    unsigned int uIdx;

    //
    // Check for valid arguments.
    //
    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        if(pcBuf[uIdx] == '\n')
        {
            if(!TX_BUFFER_FULL)
            {
                g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = '\r';
                ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
            }
            else
            {
                //
                // Buffer is full - discard remaining characters and return.
                //
                break;
            }
        }

        //
        // Send the character to the UART output.
        //
        if(!TX_BUFFER_FULL)
        {
            g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = pcBuf[uIdx];
            ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
        }
        else
        {
            //
            // Buffer is full - discard remaining characters and return.
            //
            break;
        }
    }

    //
    // If we have anything in the buffer, make sure that the UART is set
    // up to transmit it.
    //
    if(!TX_BUFFER_EMPTY)
    {
        UARTPrimeTransmit(uartstdio_device.uart);
        LL_USART_EnableIT_TC(uartstdio_device.uart);

    }

    //
    // Return the number of characters written.
    //
    return(uIdx);

}

//*****************************************************************************
//
//! A simple UART based get string function, with some line processing.
//!
//! \param pcBuf points to a buffer for the incoming string from the UART.
//! \param ui32Len is the length of the buffer for storage of the string,
//! including the trailing 0.
//!
//! This function will receive a string from the UART input and store the
//! characters in the buffer pointed to by \e pcBuf.  The characters will
//! continue to be stored until a termination character is received.  The
//! termination characters are CR, LF, or ESC.  A CRLF pair is treated as a
//! single termination character.  The termination characters are not stored in
//! the string.  The string will be terminated with a 0 and the function will
//! return.
//!
//! In both buffered and unbuffered modes, this function will block until
//! a termination character is received.  If non-blocking operation is required
//! in buffered mode, a call to UARTPeek() may be made to determine whether
//! a termination character already exists in the receive buffer prior to
//! calling UARTgets().
//!
//! Since the string will be null terminated, the user must ensure that the
//! buffer is sized to allow for the additional null character.
//!
//! \return Returns the count of characters that were stored, not including
//! the trailing 0.
//
//*****************************************************************************
int
UARTgets(char *pcBuf, uint32_t ui32Len)
{

    uint32_t ui32Count = 0;
    int8_t cChar;



    //
    // Adjust the length back by 1 to leave space for the trailing
    // null terminator.
    //
    ui32Len--;

    //
    // Process characters until a newline is received.
    //
    while(1)
    {
        //
        // Read the next character from the receive buffer.
        //
        if(!RX_BUFFER_EMPTY)
        {
            cChar = g_pcUARTRxBuffer[g_ui32UARTRxReadIndex];
            ADVANCE_RX_BUFFER_INDEX(g_ui32UARTRxReadIndex);

            //
            // See if a newline or escape character was received.
            //
            if((cChar == '\r') || (cChar == '\n') || (cChar == 0x1b))
            {
                //
                // Stop processing the input and end the line.
                //
                break;
            }

            //
            // Process the received character as long as we are not at the end
            // of the buffer.  If the end of the buffer has been reached then
            // all additional characters are ignored until a newline is
            // received.
            //
            if(ui32Count < ui32Len)
            {
                //
                // Store the character in the caller supplied buffer.
                //
                pcBuf[ui32Count] = cChar;

                //
                // Increment the count of characters received.
                //
                ui32Count++;
            }
        }
    }

    //
    // Add a null termination to the string.
    //
    pcBuf[ui32Count] = 0;

    //
    // Return the count of int8_ts in the buffer, not counting the trailing 0.
    //
    return(ui32Count);


}

//*****************************************************************************
//
//! Read a single character from the UART, blocking if necessary.
//!
//! This function will receive a single character from the UART and store it at
//! the supplied address.
//!
//! In both buffered and unbuffered modes, this function will block until a
//! character is received.  If non-blocking operation is required in buffered
//! mode, a call to UARTRxAvail() may be made to determine whether any
//! characters are currently available for reading.
//!
//! \return Returns the character read.
//
//*****************************************************************************
unsigned char
UARTgetc(void)
{

    unsigned char cChar;

    //
    // Wait for a character to be received.
    //
    while(RX_BUFFER_EMPTY)
    {
        //
        // Block waiting for a character to be received (if the buffer is
        // currently empty).
        //
    }

    //
    // Read a character from the buffer.
    //
    cChar = g_pcUARTRxBuffer[g_ui32UARTRxReadIndex];
    ADVANCE_RX_BUFFER_INDEX(g_ui32UARTRxReadIndex);

    //
    // Return the character to the caller.
    //
    return(cChar);

}

//*****************************************************************************
//
//! A simple UART based vprintf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param vaArgP is a variable argument list pointer whose content will depend
//! upon the format string passed in \e pcString.
//!
//! This function is very similar to the C library <tt>vprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments in the variable arguments list must match the
//! requirements of the format string.  For example, if an integer was passed
//! where a string was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void
UARTvprintf(const char *pcString, va_list vaArgP)
{
    uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;

    //
    // Check the arguments.
    //


    //
    // Loop while there are more characters in the string.
    //
    while(*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ui32Idx = 0;
            (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
            ui32Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        UARTwrite(pcString, ui32Idx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ui32Idx;

        //
        // See if the next character is a %.
        //
        if(*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            //
            ui32Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*pcString++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((pcString[-1] == '0') && (ui32Count == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ui32Count *= 10;
                    ui32Count += pcString[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Print out the character.
                    //
                    UARTwrite((char *)&ui32Value, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d and %i commands.
                //
                case 'd':
                case 'i':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((int32_t)ui32Value < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ui32Value = -(int32_t)ui32Value;

                        //
                        // Indicate that the value is negative.
                        //
                        ui32Neg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcStr, ui32Idx);

                    //
                    // Write any required padding spaces
                    //
                    if(ui32Count > ui32Idx)
                    {
                        ui32Count -= ui32Idx;
                        while(ui32Count--)
                        {
                            UARTwrite(" ", 1);
                        }
                    }

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 16.
                    //
                    ui32Base = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value) &&
                         (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ui32Neg)
                    {
                        ui32Count--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ui32Neg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ui32Count > 1) && (ui32Count < 16))
                    {
                        for(ui32Count--; ui32Count; ui32Count--)
                        {
                            pcBuf[ui32Pos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ui32Neg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ui32Idx; ui32Idx /= ui32Base)
                    {
                        pcBuf[ui32Pos++] =
                            g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcBuf, ui32Pos);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    UARTwrite(pcString - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    UARTwrite("ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }
}

//*****************************************************************************
//
//! A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! format string.
//!
//! This function is very similar to the C library <tt>fprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments after \e pcString must match the requirements of
//! the format string.  For example, if an integer was passed where a string
//! was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void
UARTprintf(const char *pcString, ...)
{
    va_list vaArgP;

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, pcString);

    UARTvprintf(pcString, vaArgP);

    //
    // We're finished with the varargs now.
    //
    va_end(vaArgP);
}

//*****************************************************************************
//
//! Returns the number of bytes available in the receive buffer.
//!
//! This function, available only when the module is built to operate in
//! buffered mode using \b UART_BUFFERED, may be used to determine the number
//! of bytes of data currently available in the receive buffer.
//!
//! \return Returns the number of available bytes.
//
//*****************************************************************************
#if defined(UART_BUFFERED) || defined(DOXYGEN)
int
UARTRxBytesAvail(void)
{
    return(RX_BUFFER_USED);
}
#endif


//*****************************************************************************
//
//! Returns the number of bytes free in the transmit buffer.
//!
//! This function, available only when the module is built to operate in
//! buffered mode using \b UART_BUFFERED, may be used to determine the amount
//! of space currently available in the transmit buffer.
//!
//! \return Returns the number of free bytes.
//
//*****************************************************************************
int
UARTTxBytesFree(void)
{
    return(TX_BUFFER_FREE);
}


//*****************************************************************************
//
//! Looks ahead in the receive buffer for a particular character.
//!
//! \param ucChar is the character that is to be searched for.
//!
//! This function, available only when the module is built to operate in
//! buffered mode using \b UART_BUFFERED, may be used to look ahead in the
//! receive buffer for a particular character and report its position if found.
//! It is typically used to determine whether a complete line of user input is
//! available, in which case ucChar should be set to CR ('\\r') which is used
//! as the line end marker in the receive buffer.
//!
//! \return Returns -1 to indicate that the requested character does not exist
//! in the receive buffer.  Returns a non-negative number if the character was
//! found in which case the value represents the position of the first instance
//! of \e ucChar relative to the receive buffer read pointer.
//
//*****************************************************************************

int
UARTPeek(unsigned char ucChar)
{
    int iCount;
    int iAvail;
    uint32_t ui32ReadIndex;

    //
    // How many characters are there in the receive buffer?
    //
    iAvail = (int)RX_BUFFER_USED;
    ui32ReadIndex = g_ui32UARTRxReadIndex;

    //
    // Check all the unread characters looking for the one passed.
    //
    for(iCount = 0; iCount < iAvail; iCount++)
    {
        if(g_pcUARTRxBuffer[ui32ReadIndex] == ucChar)
        {
            //
            // We found it so return the index
            //
            return(iCount);
        }
        else
        {
            //
            // This one didn't match so move on to the next character.
            //
            ADVANCE_RX_BUFFER_INDEX(ui32ReadIndex);
        }
    }

    //
    // If we drop out of the loop, we didn't find the character in the receive
    // buffer.
    //
    return(-1);
}


//*****************************************************************************
//
//! Flushes the receive buffer.
//!
//! This function, available only when the module is built to operate in
//! buffered mode using \b UART_BUFFERED, may be used to discard any data
//! received from the UART but not yet read using UARTgets().
//!
//! \return None.
//
//*****************************************************************************

void
UARTFlushRx(void)
{
    uint32_t ui32Int;

    //
    // Temporarily turn off interrupts.
    //
    ui32Int = NVIC_GetEnableIRQ(uartstdio_device.uart_irqn);
    if (ui32Int) NVIC_DisableIRQ(uartstdio_device.uart_irqn);

    //
    // Flush the receive buffer.
    //
    g_ui32UARTRxReadIndex = 0;
    g_ui32UARTRxWriteIndex = 0;

    //
    // If interrupts were enabled when we turned them off, turn them
    // back on again.
    //
    if(ui32Int)
    {
        NVIC_EnableIRQ(uartstdio_device.uart_irqn);
    }
}


//*****************************************************************************
//
//! Flushes the transmit buffer.
//!
//! \param bDiscard indicates whether any remaining data in the buffer should
//! be discarded (\b true) or transmitted (\b false).
//!
//! This function, available only when the module is built to operate in
//! buffered mode using \b UART_BUFFERED, may be used to flush the transmit
//! buffer, either discarding or transmitting any data received via calls to
//! UARTprintf() that is waiting to be transmitted.  On return, the transmit
//! buffer will be empty.
//!
//! \return None.
//
//*****************************************************************************

void
UARTFlushTx(bool bDiscard)
{
    uint32_t ui32Int;

    //
    // Should the remaining data be discarded or transmitted?
    //
    if(bDiscard)
    {
        //
        // The remaining data should be discarded, so temporarily turn off
        // interrupts.
        //
        ui32Int = NVIC_GetEnableIRQ(uartstdio_device.uart_irqn);
        if (ui32Int) NVIC_DisableIRQ(uartstdio_device.uart_irqn);

        //
        // Flush the transmit buffer.
        //
        g_ui32UARTTxReadIndex = 0;
        g_ui32UARTTxWriteIndex = 0;

        //
        // If interrupts were enabled when we turned them off, turn them
        // back on again.
        //
        if(ui32Int)
        {

            if (ui32Int) NVIC_DisableIRQ(uartstdio_device.uart_irqn);
        }
    }
    else
    {
        //
        // Wait for all remaining data to be transmitted before returning.
        //
        while(!TX_BUFFER_EMPTY)
        {
        }
    }
}


//*****************************************************************************
//
//! Enables or disables echoing of received characters to the transmitter.
//!
//! \param bEnable must be set to \b true to enable echo or \b false to
//! disable it.
//!
//! This function, available only when the module is built to operate in
//! buffered mode using \b UART_BUFFERED, may be used to control whether or not
//! received characters are automatically echoed back to the transmitter.  By
//! default, echo is enabled and this is typically the desired behavior if
//! the module is being used to support a serial command line.  In applications
//! where this module is being used to provide a convenient, buffered serial
//! interface over which application-specific binary protocols are being run,
//! however, echo may be undesirable and this function can be used to disable
//! it.
//!
//! \return None.
//
//*****************************************************************************

void
UARTEchoSet(bool bEnable)
{
    uartstdio_device.g_bDisableEcho = !bEnable;
}


//*****************************************************************************
//
//! Handles UART interrupts.
//!
//! This function handles interrupts from the UART.  It will copy data from the
//! transmit buffer to the UART transmit FIFO if space is available, and it
//! will copy data from the UART receive FIFO to the receive buffer if data is
//! available.
//!
//! \return None.
//
//*****************************************************************************

void UARTStdioIntHandler(void)
{
//	led_command_t	Switch_Task_led_command;
//	BaseType_t	xHigherPriorityTaskWoken = pdFALSE;
//    int8_t cChar;
//
//    static bool bLastWasCR = false;
    USART_TypeDef * std_uart = uartstdio_device.uart;


    //
    // Are we being interrupted because the TX FIFO has space available?
    //
    if(LL_USART_IsActiveFlag_TC(std_uart))
    {
    	LL_USART_ClearFlag_TC(std_uart);
        //
        // Move as many bytes as we can into the transmit FIFO.
        //
        UARTPrimeTransmit();

        //
        // If the output buffer is empty, turn off the transmit interrupt.
        //
        if(TX_BUFFER_EMPTY)
        {
        	LL_USART_DisableIT_TC(std_uart);
        }
    }

    //
    // Are we being interrupted due to a received character?
    //
    if(LL_USART_IsActiveFlag_RXNE(std_uart))
    {
//    	cChar = LL_USART_ReceiveData8(std_uart);
//    	if (cChar == 't')
//    	{
//    		Switch_Task_led_command = LED_TOGGLE;
//    		  xQueueSendFromISR(queue_led_command,&Switch_Task_led_command, &xHigherPriorityTaskWoken);
//    		  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//    	}

    }

}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

