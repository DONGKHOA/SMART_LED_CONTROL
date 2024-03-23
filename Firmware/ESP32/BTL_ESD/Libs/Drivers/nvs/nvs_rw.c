#include "nvs_rw.h"

static esp_err_t err;
static const char *TAG = "NVS";

/**
 * The function `NVS_Init` initializes the NVS (Non-Volatile Storage) and handles cases where the NVS
 * partition needs to be erased and reinitialized.
 * 
 * @return The function `NVS_Init()` is returning an `esp_err_t` type, which is typically used in ESP32
 * development for error handling. In this case, the function is returning `ESP_OK` if the
 * initialization of the NVS (Non-Volatile Storage) is successful.
 */
esp_err_t NVS_Init()
{
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    return ESP_OK;
}

/**
 * The function `NVS_Open` opens a Non-Volatile Storage (NVS) handle for reading and writing.
 * 
 * @param my_handle The `my_handle` parameter in the `NVS_Open` function is a pointer to a variable of
 * type `nvs_handle_t`. This pointer is used to store the handle to the Non-Volatile Storage (NVS) that
 * is opened in the function. By passing a pointer to `n
 */
void NVS_Open(nvs_handle_t *my_handle)
{
    ESP_LOGI(TAG, "\nOpening Non-Volatile Storage (NVS) handle... ");
    err = nvs_open("storage", NVS_READWRITE, my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "Done\n");
    }  
}

void NVS_Read(nvs_handle_t *my_handle, TYPE_Data_t type, int64_t *data)
{
    ESP_LOGI(TAG, "Reading data from NVS ... ");
    switch (type)
    {
    case UNSIGNED_INTEGER_8:
        err = nvs_get_u8(*my_handle, "NVS_user", (uint8_t *)data);
        break;

    case UNSIGNED_INTEGER_16:
        err = nvs_get_u16(*my_handle, "NVS_user", (uint16_t *)data);
        break;

    case UNSIGNED_INTEGER_32:
        err = nvs_get_u32(*my_handle, "NVS_user", (uint32_t *)data);
        break;
        
    case UNSIGNED_INTEGER_64:
        err = nvs_get_u64(*my_handle, "NVS_user", (uint64_t *)data);
        break;

    case SIGNED_INTEGER_8:
        err = nvs_get_i8(*my_handle, "NVS_user", (int8_t *)data);
        break;

    case SIGNED_INTEGER_16:
        err = nvs_get_i16(*my_handle, "NVS_user", (int16_t *)data);
        break;

    case SIGNED_INTEGER_32:
        err = nvs_get_i32(*my_handle, "NVS_user", (int32_t *)data);
        break;
        
    default:
        err = nvs_get_i64(*my_handle, "NVS_user", data);
        break;
    }

    switch (err) {
        case ESP_OK:
            ESP_LOGI(TAG, "Done\n");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGE(TAG, "The value is not initialized yet!\n");
            break;
        default :
            ESP_LOGE(TAG, "Error (%s) reading!\n", esp_err_to_name(err));
    }
}

void NVS_Write(nvs_handle_t *my_handle, TYPE_Data_t type, int64_t *data)
{
    ESP_LOGI(TAG, "Updating data in NVS ... ");
    switch (type)
    {
    case UNSIGNED_INTEGER_8:
        err = nvs_set_u8(*my_handle, "NVS_user", (uint8_t)*data);
        break;

    case UNSIGNED_INTEGER_16:
        err = nvs_set_u16(*my_handle, "NVS_user", (uint16_t)*data);
        break;

    case UNSIGNED_INTEGER_32:
        err = nvs_set_u32(*my_handle, "NVS_user", (uint32_t)*data);
        break;
        
    case UNSIGNED_INTEGER_64:
        err = nvs_set_u64(*my_handle, "NVS_user", (uint64_t)*data);
        break;

    case SIGNED_INTEGER_8:
        err = nvs_set_i8(*my_handle, "NVS_user", (int8_t)*data);
        break;

    case SIGNED_INTEGER_16:
        err = nvs_set_i16(*my_handle, "NVS_user", (int16_t)*data);
        break;

    case SIGNED_INTEGER_32:
        err = nvs_set_i32(*my_handle, "NVS_user", (int32_t)*data);
        break;
        
    default:
        err = nvs_set_i64(*my_handle, "NVS_user", *data);
        break;
    }
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    ESP_LOGI(TAG, "Committing updates in NVS ... ");
    err = nvs_commit(*my_handle);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    // Close
    nvs_close(*my_handle);
}