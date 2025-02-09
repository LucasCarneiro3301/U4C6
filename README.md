# U4C6
Repositório para a atividade da unidade 4 e capítulo 6: Introdução às Interfaces de Comunicação Serial com RP2040 UART, SPI e I2C

__Aluno:__
Lucas Carneiro de Araújo Lima

## ATIVIDADE 
__Descrição:__
Este projeto implementa a comunicação entre um _display_ SSD1306, LEDs WS2812 e um monitor serial, permitindo a exibição de caracteres enviados pelo PC e a interação com botões físicos. A biblioteca font.h foi modificada para incluir caracteres minúsculos, melhorando a representação no _display_ 128x64 via I2C. Além disso, números digitados no monitor serial são convertidos em símbolos exibidos na matriz de LEDs WS2812.

Os botões A e B controlam os LEDs RGB verde e azul, respectivamente, com estados alternados por interrupções (IRQ) e tratamento de bouncing via software.

<div align="center">
  <img src="https://github.com/user-attachments/assets/3f5fca4a-7a82-45b3-a54a-49796904e402" alt="GIF demonstrativo" width="400"/>
</div>

__Para este trabalho, os seguintes componentes e ferramentas se fazem necessários:__
1) Microcontrolador Raspberry Pi Pico W.
3) Ambiente de trabalho VSCode.
4) 2 LEDs
6) 2 Botões Pull-Up
7) _display_ SSD1306
8) Matriz de LEDs 5x5 WS2812
9) Cabo USB para comunicação serial
10) Ferramenta educacional BitDogLab.

## Instruções de Uso

### 1. Clone o repositório
Abra o terminal e execute o comando abaixo para clonar o repositório em sua máquina:
```bash
git clone https://github.com/LucasCarneiro3301/U4C6.git
```

### 2. Configure o ambiente de desenvolvimento
Certifique-se de que o [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk) esteja instalado e configurado corretamente no seu sistema.

### 3. Conexão com a Rapberry Pico
1. Conecte o Raspberry Pi Pico ao seu computador via USB.
2. Inicie o modo de gravação pressionando o botão **BOOTSEL** e **RESTART**.
3. O Pico será montado como um dispositivo de armazenamento USB.
4. Execute através do comando **RUN** a fim de copiar o arquivo `U4C6.uf2` para o Pico.
5. O Pico reiniciará automaticamente e executará o programa.

### 4. Observações (IMPORTANTE !!!)
1. Manuseie a placa com cuidado.
2. Certifique-se de realizar o monitoramento serial.
3. Verifique o valor da porta e o valor de _baude rate_ (115200).

## Recursos e Funcionalidades

### 1. Botões

| BOTÃO                            | DESCRIÇÃO                                     | 
|:----------------------------------:|:---------------------------------------------:|
| A                                  | Altera o estado atual do LED Verde                  | 
| B                                  | Altera o estado atual do LED Azul                | 

### 2. LEDs

Os LED verde e azul alternam de estado (aceso ou apagado) conforme cada botão. O estado de cada LED pode ser monitorado no _display_. Uma mensagem é enviada ao monitor serial a cada alteração de estado.

### 3. Entrada de caracteres via PC

É possível receber caracteres (0-9, A-Z e a-z) como entrada via teclado do computador. O caractere recebido é ilustrado no _display_. Caso o caractere seja um número, este, além de aparecer no _display_, também é ilustrado na matriz de LEDs.

### 4. Interrupções

O programa implementa interrupção nos botões A e B. Dessa forma, não é necessário ler continuamente o estado de cada botão.

### 5. Deboucing

O programa implementa tratamento de boucing (via software). Dessa forma, evita leituras falsas de botões causadas por oscilações elétricas, garantindo que cada pressionamento seja registrado apenas uma vez.

### 6. Modificação da Biblioteca font.h

Foi adicionado padrões para as letras mínusculas na biblioteca `font.h`. Além disso, também foi adicionado o caractere ':' (dois pontos).

### 7. Display SSD1306

Conectado via I2C, o display conta com a seguinte interface:

<div align="center">
  <img src="https://github.com/user-attachments/assets/76f7e683-0953-4a95-86ee-4ec721806d74" alt="GIF demonstrativo" width="400"/>
</div>

Em que **VERDE:** ilustra o estado atual do LED verde (0 para apagado e 1 para aceso), **AZUL:** ilustra o estado atual do LED azul (0 para apagado e 1 para aceso) e **LETRA:** ilustra o último caractere recebido como entrada.












