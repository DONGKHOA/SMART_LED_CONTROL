#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

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
esp_err_t NVS_Init(void)
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
 * The function `NVS_WriteString` writes a string value to a non-volatile storage (NVS) with error
 * handling.
 * 
 * @param name The `name` parameter in the `NVS_WriteString` function is used to specify the namespace
 * under which the key-value pair will be stored in the Non-Volatile Storage (NVS) system. It helps in
 * organizing and accessing data within the NVS.
 * @param key The `key` parameter in the `NVS_WriteString` function is a unique identifier for the data
 * you want to store in the Non-Volatile Storage (NVS). It is used to retrieve the stored data later
 * when needed.
 * @param stringVal The `stringVal` parameter in the `NVS_WriteString` function is the string value
 * that you want to write to the Non-Volatile Storage (NVS) under the specified key. This function
 * opens an NVS handle, sets the string value associated with the given key, commits the changes
 * 
 * @return The function `NVS_WriteString` is returning an `esp_err_t` type, which is the error code
 * indicating the success or failure of the operation.
 */
esp_err_t NVS_WriteString(const char* name, const char* key, 
                            const char* stringVal)
{
    nvs_handle_t nvsHandle;
    esp_err_t retVal;
 
    retVal = nvs_open(name, NVS_READWRITE, &nvsHandle);
    if(retVal != ESP_OK)
    {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle for Write", esp_err_to_name(retVal));
    }
    else
    {
        printf("opening NVS Write handle Done \r\n");
        retVal = nvs_set_str(nvsHandle, key, stringVal);
        if(retVal != ESP_OK)
        {
            ESP_LOGE("NVS", "Error (%s) Can not write/set value: %s", esp_err_to_name(retVal), stringVal);
        }
 
        retVal = nvs_commit(nvsHandle);
        if(retVal != ESP_OK)
        {
            ESP_LOGE("NVS", "Error (%s) Can not commit - write", esp_err_to_name(retVal));
        }
        else
        {
            ESP_LOGI("NVS", "Write Commit Done!");
        }
       
    }

    nvs_close(nvsHandle);
    return retVal;
}

esp_err_t NVS_Read_String(const char* name, const char* key, char* savedData)
{
    nvs_handle_t nvsHandle;
    esp_err_t retVal;
    uint8_t len = 32;

    ESP_LOGW("NVS", "Show Value-> name: %s, key: %s, len: %d", name, key, len);
 
    retVal = nvs_open(name, NVS_READWRITE, &nvsHandle);
    if(retVal != ESP_OK)
    {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle for Write", esp_err_to_name(retVal));
    }
    else
    {
        printf("opening NVS Read handle Done \r\n");
        retVal = nvs_get_str(nvsHandle, key, savedData, &len);
        if(retVal == ESP_OK)
        {
            ESP_LOGW("NVS", "*****(%s) Can read/get value: %s", esp_err_to_name(retVal), savedData);
        }
        else
        {
            ESP_LOGE("NVS", "Error (%s) Can not read/get value: %s", esp_err_to_name(retVal), savedData);
        }

        retVal = nvs_commit(nvsHandle);
        if(retVal != ESP_OK)
        {
            ESP_LOGE("NVS", "Error (%s) Can not commit - read", esp_err_to_name(retVal));
        }
        else
        {
            ESP_LOGI("NVS", "Read Commit Done!");
        }
    }
 
    nvs_close(nvsHandle);
    return retVal;
}