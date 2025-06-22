# Módulo IoT - Projeto Waterline

Este documento descreve a seção de Internet das Coisas (IoT) do projeto Waterline, responsável pelo monitoramento e controle em tempo real de um ecossistema de aquário. O sistema utiliza um microcontrolador ESP8266 como cérebro principal para conectividade Wi-Fi e um Arduino Uno como controlador auxiliar para os periféricos.

## Diagrama do Circuito

A imagem abaixo ilustra todas as conexões de hardware entre os microcontroladores, sensores e atuadores.

![Diagrama do Circuito Waterline](./docs/img/Diagrama%20-%20Waterline%20IOT.png)

## Componentes Utilizados

### Hardware Principal

- **Microcontrolador Principal:** NodeMCU (ESP8266) - Responsável pela conectividade Wi-Fi, comunicação com a nuvem (AWS IoT) e controle de sensores específicos.
- **Microcontrolador Auxiliar:** Arduino Uno - Atua como um expansor de I/O, controlando o módulo de relés de 4 canais e fornecendo os 5 volts necessários.

### Sensores

- **Sensor de Nível de Água:** Módulo analógico para medição do nível da água no aquário.
- **Sensor de Temperatura da Água:** DS18B20, sensor digital à prova d'água de alta precisão.
- **Sensor de Temperatura e Umidade do Ar:** Módulo DHT11 para monitorar as condições do ambiente.

### Atuadores e Módulos

- **Módulo Relé de 4 Canais:** Controla os dispositivos de maior potência (iluminação, bomba, filtro).
- **Módulo Relé de 1 Canal:** Utilizado especificamente para o pressionamento do botão físico do alimentador automático.

### Cargas (Dispositivos Controlados)

- Iluminação
- Bomba d'água
- Filtro
- Alimentador Automático

## Funcionalidades

- **Monitoramento em Tempo Real:** Leitura contínua dos sensores e envio dos dados para uma plataforma em nuvem IoT Core, permitindo a visualização em um dashboard web.
- **Controle Remoto:** Acionamento individual de dispositivos como a iluminação, bomba e filtro através de comandos enviados pela internet.
- **Alimentação Automatizada:** Capacidade de acionar o ciclo de alimentação do dispenser de forma remota ou programada.

## Conexões (Pinagem)

A seguir, um resumo das principais conexões, conforme o diagrama.

### Conexões no ESP8266 (NodeMCU)

| Componente                            | Pino no ESP8266 | Conexão             |
| ------------------------------------- | --------------- | ------------------- |
| Sensor de Nível de Água               | A0              | Saída Analógica (S) |
| Relé do Alimentador (1 Canal)         | D0              | Sinal (IN)          |
| Sensor temperatura ambiente e umidade | D2              | Sinal (IN)          |
| Sensor nível da água                  | D3              | Sinal (IN)          |
| Sensor de Temperatura (DS18B20)       | D4              | Pino de Dados       |
| Relé 1                                | D5              | Sinal (OUT)         |
| Relé 2                                | D6              | Sinal (OUT)         |
| Relé 3                                | D7              | Sinal (OUT)         |
| Relé 4                                | D8              | Sinal (OUT)         |

### Conexões no Arduino Uno

| Componente            | Pino no Arduino | Conexão         |
| --------------------- | --------------- | --------------- |
| VCC do módulo 4 relés | 5v              | Pino de energia |
| GND do módulo 4 relés | GND             | Pino terra      |

## Arquitetura e Lógica de Funcionamento

O **ESP8266** atua como o cérebro do sistema. Ele se conecta à rede Wi-Fi e estabelece comunicação via MQTT com a plataforma da AWS IoT Core. Ele é responsável por ler os sensores que lhe são diretamente conectados (nível e temperatura da água) e por enviar comandos para o Arduino.

O **Arduino Uno** funciona como um "ajudante" robusto, simplificando o controle dos relés de 5V. O ESP8266 trabalha apenas com 3,3v, que não é o suficiente para ativar os relés.

O sistema de **alimentação** utiliza um relé de 1 canal para fechar o circuito do botão físico do alimentador, simulando um pressionamento de dedo de forma automatizada.
