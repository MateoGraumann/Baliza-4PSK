# Baliza con Modulación 4-PSK

Este proyecto consiste en una baliza que emite un mensaje mediante una modulación de 4-PSK, la cual es una modulación de fase con 4 símbolos. Esta baliza presenta las siguientes características:

- **Payload**: 50 bits (incluye señal indicativa, ubicación y potencia).
- **Constelación de símbolos**: 4.
- **Número de símbolos por payload**: 162.
- **Modulación**: 4-PSK con una separación de tonos de 1.4648 Hz.
- **Duración del mensaje**: 110.6 segundos.
- **Duración por símbolo**: 8192/12000 segundos (0.6827 segundos por símbolo).

Los subsistemas utilizados en este trabajo incluyen un generador de mensajes, un DDS (SI5351), Raspberry Pi Pico W, y un filtro pasa bajos.

## Generador de mensajes o módulo de control

Como módulo de control se utilizó una **Raspberry Pi Pico W** y, junto con el IDE de Arduino, se realizó un código para gestionar la sincronización del tiempo, el reloj interno, los mensajes a transmitir, la modulación del DDS y los mensajes de registro (log) para monitorear el estado de ejecución del dispositivo.

## DDS

Para el DDS se utilizó la placa **SI5351**, que tenía una frecuencia base de 7,030,600 Hz emitida por el CLK0. El CLK1 generaba la sumatoria de la frecuencia base, una frecuencia de desplazamiento aleatoria entre 0 y 2500 Hz, y la multiplicación del símbolo a transmitir con la frecuencia de separación de tonos (1.4648 Hz).

Para su comunicación con el módulo de control se utilizó el protocolo **I2C** y los pines SCL y SDA por defecto, que son el **GPIO 4** y **GPIO 5**.

## Filtro pasa bajos

El filtro pasa bajos utilizado en este proyecto fue un **filtro pi de 3 etapas**.
