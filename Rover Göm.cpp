#include "main.h"

// UART ve GPIO yapılandırmaları
UART_HandleTypeDef huart1;  // UART yapılandırması
GPIO_InitTypeDef GPIO_InitStruct = {0};  // GPIO yapılandırması

// UART'tan alınacak veriyi saklayacak buffer
uint8_t rx_data[2];  // UART'tan alınacak 2 byte veri

// GPIO pin tanımlamaları
#define GPIO_PIN_0  GPIO_PIN_0  // PA0
#define GPIO_PIN_1  GPIO_PIN_1  // PA1
#define GPIO_PORT   GPIOA       // GPIOA portu

// UART mesajını alıp GPIO pinlerini kontrol etmek için fonksiyon
void UART_ReceiveAndControlGPIO(void) {
    // UART'tan 2 byte veri alıyoruz
    HAL_UART_Receive(&huart1, rx_data, 2, HAL_MAX_DELAY);

    // GPIO Pin 0 kontrolü
    switch(rx_data[0]) {
        case '0':  // LOW durumu
            HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN_0, GPIO_PIN_RESET);  // PA0 LOW
            break;
        case '1':  // HIGH durumu
            HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN_0, GPIO_PIN_SET);    // PA0 HIGH
            break;
        case '2':  // TOGGLE durumu
            HAL_GPIO_TogglePin(GPIO_PORT, GPIO_PIN_0);  // PA0 TOGGLE
            break;
        default:
            break;
    }

    // GPIO Pin 1 kontrolü
    switch(rx_data[1]) {
        case '0':  // LOW durumu
            HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN_1, GPIO_PIN_RESET);  // PA1 LOW
            break;
        case '1':  // HIGH durumu
            HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN_1, GPIO_PIN_SET);    // PA1 HIGH
            break;
        case '2':  // TOGGLE durumu
            HAL_GPIO_TogglePin(GPIO_PORT, GPIO_PIN_1);  // PA1 TOGGLE
            break;
        default:
            break;
    }
}

// GPIO yapılandırması
void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // GPIOA saati aç

    // GPIOA Pin 0 ve Pin 1 çıkış olarak yapılandırma
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;  // PA0 ve PA1
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull çıkış modu
    GPIO_InitStruct.Pull = GPIO_NOPULL;  // Pull-up veya Pull-down yok
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // Hızlı çıkış
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  // GPIOA yapılandırmasını uygula
}

// UART yapılandırması
void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;  // UART1 portu
    huart1.Init.BaudRate = 9600;  // Baud rate 9600
    huart1.Init.WordLength = UART_WORDLENGTH_8B;  // 8 bit veri uzunluğu
    huart1.Init.StopBits = UART_STOPBITS_1;  // 1 stop biti
    huart1.Init.Parity = UART_PARITY_NONE;  // Parite yok
    huart1.Init.Mode = UART_MODE_TX_RX;  // Hem gönderme (TX) hem alma (RX)
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // Donanım akış kontrolü yok
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;  // 16x oversampling
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();  // Hata durumunda Error_Handler çağır
    }
}

// Hata yönetimi
void Error_Handler(void) {
    __disable_irq();  // Interrupt'ları devre dışı bırak
    while (1) {
        // Hata durumunda sonsuz döngü
    }
}

int main(void) {
    HAL_Init();  // HAL kütüphanesini başlat
    MX_GPIO_Init();  // GPIO pinlerini başlat
    MX_USART1_UART_Init();  // UART1 yapılandırmasını başlat

    while (1) {
        UART_ReceiveAndControlGPIO();  // UART'tan veri al ve GPIO pinlerini kontrol et
    }
}

