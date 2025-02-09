/*
  Aluno: Lucas Carneiro de Araújo Lima
*/

// Definição das bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/pio.h"
#include "U4C6.pio.h"
#include "numbers.h"

#define I2C_PORT i2c1 // Define que o barramento I2C usado será o "i2c1"
#define I2C_SDA 14 // Define que o pino GPIO 14 será usado como SDA (linha de dados do I2C)
#define I2C_SCL 15 // Define que o pino GPIO 15 será usado como SCL (linha de clock do I2C)
#define endereco 0x3C // Define o endereço I2C do dispositivo (0x3C é o endereço padrão de muitos displays OLED SSD1306)

#define WS2812_PIN 7 // Matriz de LEDs 5x5
#define BTNA 5 // Botão A
#define BTNB 6 // Botão B
const uint green = 11; // LED Verde
const uint blue = 12; // LED azul

#define NUM_PIXELS 25 // 5x5 = 25
#define IS_RGBW false

bool gstate = false; // Estado do LED verde
bool bstate = false; // Estado do LED azul

char buffer[16]; // Buffer de string
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

static void setup(); // Prototipação da função que define os LEDs RGB como saídas e os botões como entradas
void i2c_setup(); // Prototipação da função que configura o I2C
void ws2812_setup(PIO pio, uint sm); // Prototipação da função que configura a matriz de LEDs 5x5 
void ssd1306_setup(ssd1306_t* ssd); // Prototipação da função que configura o display ssd1306
void frame(ssd1306_t* ssd); // Prototipação da função que escreve a interface fixa no display ssd1306  
static void gpio_irq_handler(uint gpio, uint32_t events); // Prototipação da função de interrupção
void set_leds_from_color(PIO pio, uint sm, uint32_t cor); // Prototipação que preenche a matriz de LEDs 5x5 com uma cor

int main() {
  ssd1306_t ssd; 
  PIO pio = pio0;
  int sm = 0;

  stdio_init_all(); // Inicialização dos recursos de entrada e saída padrão
  setup(); // Inicialização e configuração dos LEDs e botões 
  i2c_setup(); // Inicialização e configuração da comunicação serial I2C 
  ws2812_setup(pio, sm); // Inicialização e configuração da matriz de LEDs 5x5
  ssd1306_setup(&ssd); // Inicializa a estrutura do display

  gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão A
  gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão b

  while (true) {
    frame(&ssd); // Gera a interface padrão no display
    int symbol = getchar_timeout_us(50); // Recebe um caractere do teclado
    if (symbol != PICO_ERROR_TIMEOUT) {
      printf("O caractere digitado foi: %c\n\n", (char) symbol);
      ssd1306_draw_char(&ssd, (char) symbol, 68, 28); // Insere o símbolo escolhido ao lado da seção "LETRA: "

      if(symbol >= 48 && symbol <= 57) numbers(symbol - 48); // Caso o símbolo seja um número, este é ilustrado na matriz de LEDs 5x5
      else set_leds_from_color(pio, sm, 0); // Caso contrário, a matriz de LEDs é limpada
    }

    ssd1306_send_data(&ssd); // Envia os dados para o display

    if(symbol == '*') {
      printf("Reiniciando para o modo de gravação!!!\n\n");
      reset_usb_boot(0,0); // Modo de gravação
    } 

    sleep_ms(10);
  }
}

// Inicializa e configura os LEDs RGB como saída. Inicializa e configura os botões como entradas.
void setup() {
  gpio_init(green);
  gpio_set_dir(green, GPIO_OUT); 
  gpio_put(green,false);

  gpio_init(blue);
  gpio_set_dir(blue, GPIO_OUT);
  gpio_put(blue,false);

  gpio_init(BTNA);
  gpio_set_dir(BTNA, GPIO_IN);
  gpio_pull_up(BTNA);  

  gpio_init(BTNB);
  gpio_set_dir(BTNB, GPIO_IN);
  gpio_pull_up(BTNB);  
}

// Inicializa e configura a comunicação serial I2C 
void i2c_setup() {
  i2c_init(I2C_PORT, 4e2 * 1e3); // Inicialização I2C.

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define a função do pino GPIO para I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Define a função do pino GPIO para I2C
  gpio_pull_up(I2C_SDA); // Pull up para linha de dados
  gpio_pull_up(I2C_SCL); // Pull up para linha de clock
}

// Inicializa e configura o display
void ssd1306_setup(ssd1306_t* ssd) {
  ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(ssd); // Configura o display
  ssd1306_send_data(ssd); // Envia os dados para o display
  ssd1306_fill(ssd, false); // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_send_data(ssd);
}

// Inicializa e configura a matriz de LEDs 5x5 
void ws2812_setup(PIO pio, uint sm) {
  uint offset = pio_add_program(pio, &U4C6_program);

  U4C6_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW); //Inicializa a matriz de leds
}

// Interface fixa do display
void frame(ssd1306_t* ssd) {
  ssd1306_rect(ssd, 0, 0, 128, 64, true, false); // Desenha um retângulo

  sprintf(buffer, "VERDE:%i", gstate);
  ssd1306_draw_string(ssd, buffer, 8, 5);

  ssd1306_draw_string(ssd, "LETRA:", 20, 28);

  sprintf(buffer, "AZUL:%i", bstate);
  ssd1306_draw_string(ssd, buffer, 8, 51);
}

// Configura a matriz de LED conforme uma cor especificada
void set_leds_from_color(PIO pio, uint sm, uint32_t cor) {
  for (int16_t i = 0; i < NUM_PIXELS; i++) {
    pio_sm_put_blocking(pio, sm, cor);
  }
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
  uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos

  if (current_time - last_time > 2e5) { // 200 ms de debouncing
      last_time = current_time; 
      if(gpio == BTNA) {
        gstate = !gstate;
        (gstate) ? printf("LED verde ligado!!!\n\n")  : printf("LED verde desligado!!!\n\n");
      } else if(gpio == BTNB) {
        bstate = !bstate;
        (bstate) ? printf("LED azul ligado!!!\n\n")  : printf("LED azul desligado!!!\n\n");
      }

      gpio_put(green, gstate);
      gpio_put(blue, bstate);
  }
}