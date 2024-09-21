## Memoria de Trabajo Final:
# MIDI Drum Kit

## Autor:
# Ing. Ian Lesnianski

# Resumen
  Este trabajo presenta el desarrollo de un instrumento percusivo electrónico.Es un sistema electrónico que captura acciones musicales mediante drum pads con transductores piezoeléctricos, los cuales convierten los golpes en señales eléctricas que se procesan y transforman en mensajes MIDI. Estos mensajes se transmiten a una computadora, donde se interpretan y se envían a una Estación de Audio Digital (DAW) para generar el sonido del instrumento.
El sistema incluye una interfaz de usuario con display gráfico, encoder rotativo y pulsadores para facilitar la configuración del drumkit, así como un pedal de control para el hi-hat.
  Este trabajo integra hardware y software para la creación de un dispositivo funcional orientado a la producción musical.

# CAPÍTULO 1
## Introducción general
### Descripción
El presente trabajo consiste en el desarrollo de un sistema electrónico orientado a la captura y procesamiento de acciones musicales mediante transductores piezoeléctricos integrados en drum pads. Estos transductores convierten la deformación mecánica generada por golpes en señales eléctricas, que luego son procesadas y transformadas en mensajes MIDI (Musical Instrument Digital Interface), siguiendo el estándar de este protocolo ampliamente utilizado en la industria musical.

La transmisión de los mensajes MIDI hacia una computadora se realizó utilizando dos opciones de comunicación: una interfaz UART o una conexión Bluetooth. Los mensajes recibidos en el puerto COM de la PC se interpretan por el software Hairless-MIDI, que se encarga de reenviar estos datos a un puerto MIDI de salida virtual, creado mediante el uso del software loopMIDI. Esto permite la interacción con una Estación de Audio Digital (DAW, por sus siglas en inglés), en este caso Reaper, que fue utilizada para generar el sonido del instrumento.

El desarrollo de este sistema involucró la captura y procesamiento de señales musicales, como también la implementación de una interfaz de usuario para facilitar la configuración de distintos atributos del drumkit. Esta interfaz incluyó un display gráfico LCD, un encoder rotativo y dos pulsadores, que permiten al usuario navegar por los menús del sistema y ajustar las configuraciones según sus necesidades. Asimismo, se añadió un pedal de control de hi-hat, mejorando la capacidad del dispositivo para emular el comportamiento de un set de batería tradicional, brindando una experiencia más realista en la ejecución musical.

Desde una perspectiva de utilidad, el sistema ofrece una aplicación directa en el ámbito de la recreación musical, permitiendo al usuario personalizar su set de drum pads y controlar los sonidos generados en tiempo real, lo que lo convierte en una herramienta de gran valor para el entretenimiento y la creación musical. Asimismo, este proyecto se destaca por ser una variante económica frente a los productos comerciales existentes en el mercado, ofreciendo una alternativa accesible.

### Diagrama conceptual

![Software empleado y vinculaciones](https://github.com/ianlesni/TPn-1-MIDI-Drum-Pad-v.0/assets/43219235/ad0bc1e4-109f-4ae2-be34-47d620211d29)
#### Referencias:
- 1: Puerto de entrada del Hairless-MIDI, recibe el mensaje enviado desde la NUCLEO-429ZI
- 2: Canal virtual que comunica al Hairless-MIDI con el Reaper
- 3: Instrumento virtual que recibe los mensajes MIDI y genera los sonidos acorde
- 4: Parámetro velocity interpretado en la consola de Debug del Hairless-MIDI y su correspondiente volumen del golpe del instrumento
- 5: Nota número 36 mapeada al pedal de bombo ejecutándose

### Diagrama en bloques del sistema

![Diagrama simplificado](https://github.com/user-attachments/assets/6ac06ecf-2314-44e4-bda9-30e8d5383e90)

### Estado del arte

Para establecer un punto de referencia, se compararon características de productos similares de marcas líderes en el mercado, como Alesis, Roland y Yamaha. Es importante mencionar que este desarrollo no buscó superar las prestaciones de los productos listados a continuación, si no tomar sus características como referencias.
El  foco del trabajo fué lograr una versión económica que tenga las prestaciónes básicas de este tipo de productos.

Marca | Alesis        | Roland | Yamaha       | 
------|---------------|--------|------------- |
Modelo| SamplePad Pro | SPD-SX | DTX Multi 12 |

Caracteristica         | SamplePad Pro           | SPD-SX                       | DTX-MULTI 12          |
-----------------------|-------------------------|------------------------------|-----------------------|
Cantidad de pads       | 8                       | 9                            | 12                    | 
Iluminación reactiva   | SI                      | SI                           | NO                    |
Control de Hi-hat      | SI                      | SI                           | SI                    |
Pedal de bombo         | SI                      | SI                           | SI                    |
MIDI in                | SI                      | SI                           | SI                    |
MIDI out               | SI                      | SI                           | SI                    |
Polifonía              | --                      | 20 voces                     | 64 voces              |
Pads dinámicos         | SI                      | SI                           | SI                    |
Memoria disponible     | SD/SDHC card externa    | 2GB                          | 168MB                 |
USB-MIDI               | SI                      | SI                           | SI                    |
Presets                | 200                     | 100                          | 50                    |
Display                | LCD Gráfico 128x64 dots | LCD Gráfico 128x64 dots      | LCD 16x2              |
SD Card Slot           | SI                      | NO                           | NO                    |
Fuente de alimentación | DC 9V                   | DC 9V                        | DC 12V                |
Precio                 | $ 1.600.000             | $2.200.500                   | $1.800.000            |

Links:
* [Alesis-SamplePad Pro](https://www.alesis.com/products/view2/samplepad-pro.html)
* [Roland-SPD-SX](https://www.roland.com/LATINAMERICA/products/spd-sx/)
* [Yamaha-DTX-MULTI 12](https://es.yamaha.com/es/products/musical_instruments/drums/el_drums/drum_kits/dtx_multi_pad/features.html)

A partir del análisis de las prestaciones expuestas se determinaron las funcionalidades escenciales del dispositivo en función de los conocimientos adquiridos a lo largo del curso, las limitaciones del hardware y la disponibilidad del mismo. Siendo el resultado un sistema de tres pads de prestaciones similares a un kit básico de batería, con capacidad de comunicación inalámbrica, interfaz de usuario y sonidos configurables.

# CAPÍTULO 2
## Introducción específica
### Requisitos

A continuación se detallan los requisitos iniciales establecidos para el trabajo final en concordancia con los criterios SMART (Specific, Measurable, Achievable, Relevant, Time-bound). Estos requisitos fueron felizmente logrados en el desarrollo.

Grupo de requisito     | ID  | Descripción                                                                                                                                                                                                 |
-----------------------|-----|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|   
1.Sensado              | 1.1 | El sistema tendrá sensibilidad a los golpes sobre los 3 drum pads, detectando la intensidad de los golpes con una resolución de 12 bits                                                                     |
  ---                  | 1.2 | El sistema contará con un pedal de control de hi-hat, cuya posición será leída con una resolución de 12 bits                                                                                                |
2.Configuraciones      | 2.1 | Podrá modificarse la nota MIDI asociada a cada uno de los drum pads de forma individual                                                                                                                     |
  ---                  | 2.2 | Podrá modificarse la sensibilidad al golpe de cada uno de los drum pads de forma individual                                                                                                                 |
  ---                  | 2.3 | Podrá modificarse la reverberación de cada uno de los drum pads de forma individual                                                                                                                         |
  ---                  | 2.4 | Podrán configurarse los canales MIDI de cada drum kit                                                                                                                                                       |
  ---                  | 2.5 |   Podrá configurarse el volumen general de cada drum kit                                                                                                                                                    |
3.Interfaz de control  | 3.1 | El sistema dispondrá de un display LCD gráfico para mostrar las configuraciones y menús                                                                                                                     |
  ---                  | 3.2 | El sistema contará con dos pulsadores: uno para confirmar las selecciones del menú y otro para cancelarlas y/o volver a la opción anterior                                                                  | 
  ---                  | 3.3 | El sistema contará con un encoder rotativo para navegar por las configuraciones y menús                                                                                                                     |
  ---                  | 3.3 | El sistema contará con un led en cada drum pad que se ilumina cada vez que recibe un golpe                                                                                                                  |
4.Conectividad         | 4.1 | El sistema podrá conectarse a la PC y enviar los mensajes MIDI mediante un cable USB, a través del conversor USB-UART de la placa Nucleo                                                                    |
  ---                  | 4.2 | El sistema podrá conectarse a la PC y enviar los mensajes MIDI mediante conexión Bluetooth                                                                                                                  |
5.Alimentación         | 5.1 | El sistema se podrá alimentarse desde el puerto USB de la PC                                                                                                                                                |
  ---                  | 5.2 | El sistema se podrá alimentarse desde un cargador con salida USB de 5V 500mA(o superior)                                                                                                                    |
6.Tiempo de desarrollo | 6.1 | El proyecto se entregará el día 17 de septiembre de 2024                                                                                                                                                    |   
7.Documentación        | 7.1 | Se entregará el código del proyecto en un repositorio de GitHub, una lista de partes, un diagrama de conexiones detallada, y una nota de oportunidades de mejora con un balance de los resultados obtenidos | 

### Casos de uso

Elemento del caso de uso | Definición                                                                                                                            |
-------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
N°                       | 1                                                                                                                                                                                 |                                    
Título                   | Generación de sonido al golpear un drum pad                                                                                                                                      |
Disparador               | El usuario golpea uno de los drum pads del equipo                                                                                                                                      |
Precondición             | El sistema está encendido, conectado a la PC vía USB o Bluetooth, en el menú de ejecución                                                                                                                                                          |
Flujo básico             | El usuario golpea un drum pad. El sistema detecta el golpe a través de un transductor piezoeléctrico. El sistema genera el mensaje MIDI correspondiente. El mensaje MIDI es enviado a la PC. El software de la PC genera el sonido correspondiente |
Flujo alternativo        | 1.a El sistema no está conectado a la PC. El drum pad detectará el golpe, pero no se generará el sonido correspondiente                                                                                                                                |

Elemento del caso de uso | Definición                                                                                                                            |
-------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
N°                       | 2  
Título                   | Configuración de nota MIDI de un drum pad                                                                                                |
Disparador               | El usuario golpea uno de los drum pads del equipo                                                                                     |
Precondición             | El sistema está encendido, conectado a la PC vía USB o Bluetooth, en el menú de configuración de drum pad                                                                                                                  |
Flujo básico             | El usuario golpea uno de los drum pads del equipo. El display indica en pantalla el nombre del drum pad que se golpeó y los atributos previamente configurados. El usuario utiliza el encoder rotativo para mover el cursor hasta el campo "MIDI Note". El usuario presiona el pulsador del encoder para editar el campo y rotando el encoder selecciona el nuevo valor de nota MIDI. El usuario presiona nuevamente el pulsador del encoder y la configuración finaliza                                                                                                                                      |
Flujo alternativo        | 2.a El usuario cancela la configuración antes de confirmar, presionando el botón back, y el sistema descarta los cambios y regresa al menú principal                                                                                                                                      |

####PONER ACA OTRO CASO DE USO

### Descripción del hardware empleado

En esta sección se plasmaran los detalles de los módulos de hardware que componen al trabajo final:
- 1: Transductor piezoeléctirco
- 2: Circuito acondicionador de señal
- 3: Sensor infrarrojo TCRT5000
- 4: Display Gráfico LCD
- 5: Encoder rotativo
- 6: Pulsaodor
- 7: Modulo BT HC-06

#### Transductor piezoeléctrico
El transductor piezoelectrico el el elemento sensor de cada uno de los drum pads. Este genera una diferencia de potencial eléctrica propocional a la intensidad del golpe, lo que permite una interpretación realista de la ejecución del instrumento.
Debido a la magnitud y caracteristicas de la señal generada por el transductor, es necesario adaptarla a los rangos de voltaje y caracteristicas de la entrada del conversor analogico-digital(ADC) previamente mediante un circuito acondicionador de señal.

![image](https://github.com/user-attachments/assets/735a64b0-05f1-485f-8945-4def7d6eeb6f)

#### Circuito acondicionador de señal
La función de este circuito es escalar y ajustar la señal proveniente del transductor piezoelectrico a valores compatibles con la entrada del ADC de la placa Nucleo. Y simultaneamente generar una interrupción para indicar que la señal proveniente del pad corresponde a un golpe sobre el instrumento y no a el ruido ambiente. Esta interrupción es la encargada de indicarle a la placa Nucleo que es momento de realizar una conversion analógica digital.

![image](https://github.com/user-attachments/assets/97758565-5580-490b-ad3d-eb135b31eeba)

#### Sensor infrarrojo TCRT5000
Este sensor infrarrojo es el componente principal del control de hi-hat. El mismo cuenta con una salida analógica proporcional a la distancia a la que se encuentra el pedal, permitiendo modificar el sonido asociado al hi-hat correspondiente a las distintas separaciones de sus platillos. Adicionalmente, el sensor cuenta con un circuito comparador configurado para proporcionar una salida digital que se activa cuando el pedal se presiona al máximo, indicando que los platillos se cierran completamente.
Esto permite un mayor realismo en la ejecución al emular de manera más precisa el comportamiento de un hi-hat tradicional.

![image](https://github.com/user-attachments/assets/ee7e3b46-d574-493f-97aa-4af5779906f0)

#### Display Gráfico LCD
El display LCD gráfico de 128x64 bits de conforma la parte visual de la interfaz con el usuario, permitiendo la visualización de las configuraciones y los distintos menús del sistema. Este display utiliza el bus SPI para comunicarse con la placa Nucleo.

![image](https://github.com/user-attachments/assets/01975f8a-5e48-4673-aa24-58b3550cf648)

### Encoder Rotativo
Se utilizó un módulo KY-040 que incluye el encoder rotativo con un pulsador integrado. Gracias a este dispositivo,el sistema cuenta con la posibilidad de navegar por las configuraciones y menús, y modificar los valores de los atributos configurables del instrumento. Este encoder complementa la interfaz de usuario y permite configurar el instrumento.

![image](https://github.com/user-attachments/assets/82967340-bac3-429d-a5dd-3dc493c77e1c)

#### Pulsador
Se hizo uso del pulsador de la placa nucleo como parte del control de la interfaz de usuario. Este pulsador permite retroceder dentro del menu y cancelar las configuraciones.

#### Modulo BT HC-06
Este módulo bluetooth permite conectar el instrumento a la PC sin la necesidad de calbes, lo que le da mayor versatilida y comodidad en el uso cotidiano. Una vez seleccionado el modo de comunicacion bluetooth, este módulo es el encargado de enviar los mensajes MIDI a la PC mediante una conxión bluetooth. El HC-06 se conecta a la placa Nucleo a traves de una interfaz UART.

![image](https://github.com/user-attachments/assets/ab1bdb41-eb6f-4fdc-93ee-600315e17eb5)


### Introducción al protocolo MIDI

#### MIDI
MIDI es un acrónimo para "Musical Instrumen Digital Interface". Es principalmente una especificación para conectar y controlar instrumentos musicales electrónicos. La especificación está propiamente detallada en el documento "MIDI 1.0 DETAILED SPECIFICATION" (disponible en https://midi.org/midi-1-0-detailed-specification).
En este proyecto el controlador MIDI envía tres tipos de mensajes MIDI. Un mensaje encargado de hacer sonar una nota y otro es el encargado de apagarla, asi como tambien un mensaje para el control de volumen del instrumento.

#### Mensajes de canal de voz 
Los mensajes de activacion y desactivación de la nota pertenecen a los mensajes de canal de voz, y están compuestos por tres bytes:

![ejemplo mensaje MIDI](https://github.com/ianlesni/TPn-1-MIDI-Drum-Pad-v.0/assets/43219235/55e81f52-99b3-476d-929b-04a91e87af98)

El primer byte de status se define con el comando Note On = 0x9 o Note Off = 0x8 y el canal MIDI. Por ejemplo, para encender o apagar el canal 0 el byte de estatus debería ser 0x90 y 0x80 respectivamente.
En el caso de este desarrollo el canal para transmitir la información requerida es configurable, pudiendo adoptar cualquiera de los 16 canales MIDI disponibles.
Cada vez que se golpea el drum pad se genera un mensaje de Note On. A qué suena ese golpe lo define el segundo byte de datos, la ""nota"" de ese mensaje MIDI. Esa nota no es una nota musical, es un número que representa un instrumento percusivo:

![Mapeo de notas midi](https://github.com/ianlesni/TPn-1-MIDI-Drum-Pad-v.0/assets/43219235/2c08b594-ac7b-4a3d-b11f-0a8a383687f4)

Por último, el tercer byte de datos es el parámetro "velocity" que define qué tan fuerte suena el instrumento virtual. En los instrumentos percusivos se asocia a la intensidad del golpe. Este valor se calcula a partir de la medición de la entrada conectada al transductor piezoeléctrico. La diferencia de potencial medida por el ADC se convierte en un valor digital, el cual se escala para producir un número en dentro del rango de 0 a 127, correspondiente al parámetro velocity del protocolo MIDI:

![Velocity](https://github.com/ianlesni/TPn-1-MIDI-Drum-Pad-v.0/assets/43219235/8a8005aa-990d-452e-abca-52719e0e45f9)

Para esta versión del desarrollo, las notas se apagan de manera alternativa, de acuerdo con el estándar MIDI. Este método consiste en enviar un mensaje MIDI con el comando Note On y una velocity de 0 (0x00) para la nota que está ejecutándose.

#### Mensajes de cambio de control
Los mensajes de cambio de control (CC) se utilizan para controlar las diversas funciones y parámetros de los instrumentos MIDI. El mensaje de control de volumen es en escencia un mensaje de cambio de control MIDI y está compuesto por tres bytes: 

![image](https://github.com/user-attachments/assets/f4beb6ba-36f9-4bb6-8643-edf217a8a40b)

El priemr byte es el byte de status compuesto por el comando de cambio de control = 0xB y el número de canal afectado por el cambio (el canal MIDI es un atributo del drumkit).
El segundo byte es el numero de control.Para esta aplicación se adopto como número de control = 0x3F o 63 en decimal, aprovechando que su función no está definida por el protocolo MIDI. 

![image](https://github.com/user-attachments/assets/ca9251c7-1490-4470-b62e-17c08bc10a5a)

Por ultimo, el tercer byte es el valor del volumen del track y es un número del 0 al 127 que se configura desde la interfaz de usuario del sistema.

# CAPÍTULO 3
## Diseño e implementación

En esta sección se detallan las caracteristicas del sistema, sus componentes y conexiones.

### Diagramas de la implementación
A continuación se presenta un diagrama en bloques detallado para comprender la interaccion entre los componentes principales y el microcontrolador, y una imagen de la implementación del sistema con sus respectivas referencias para una asociación directa con el diagrama en bloques.

![Diagrama en bloques](https://github.com/user-attachments/assets/f28c5ccb-20cd-4d31-93cf-401ee9897b42)


![Implementacion](https://github.com/user-attachments/assets/06fff157-ff0f-4d7d-8673-89ec883d4c52)

#### Referencias
- 1: Placa Nucleo F429ZI
- 2: PC ejecutando los programas Hairless-MIDI, loopMIDI y Reaper
- 3: Interfaz de control compuesta por un encoder, el pulsador integrado en la placa Nucleo y un display GLCD
- 4: Drumpads y circuito acondicionador de señal
- 5: Modulo Bluetooth
- 6: Pedal de control de hi-hat

### Conexiones con la placa Nucleo 
En esta subsección se encuentra la asignación de pines para cada módulo que compone al drumkit.

DRUMPAD 0       | Nucleo - F429ZI |  
----------------|-----------------|
Vout            | PC_3            | 
Int             | PE_6            | 
LED             | PB_0            | 
3,3 V           | 3,3 V           | 
GND             | GND             | 

DRUMPAD 1       | Nucleo - F429ZI |  
----------------|-----------------|
Vout            | PA_3            | 
Int             | PF_9            | 
LED             | PB_7            | 
3,3 V           | 3,3 V           | 
GND             | GND             | 

DRUMPAD 2       | Nucleo - F429ZI |  
----------------|-----------------|
Vout            | PF_3            | 
Int             | PG_1            | 
LED             | PB_14           | 
3,3 V           | 3,3 V           | 
GND             | GND             | 

TCRT 5000       | Nucleo - F429ZI |  
----------------|-----------------|
D0              | PF_7            | 
A0              | PC_0            | 
3,3 V           | 3,3 V           | 
GND             | GND             | 

HC - 06         | Nucleo - F429ZI |  
----------------|-----------------|
RX              | PD_5            | 
TX              | PD_6            | 
+5 V            | 5 V             | 
GND             | GND             | 

HY - 040        | Nucleo - F429ZI |  
----------------|-----------------|
CLK             | PF_8            | 
DT              | PE_3            | 
SW              | PG_14           | 
3,3 V           | 3,3 V           | 
GND             | GND             | 

PULSADOR        | Nucleo - F429ZI |  
----------------|-----------------|
USER            | PC_13           |

GLCD            | Nucleo - F429ZI |  
----------------|-----------------|
RS              | PD_10           | 
R/W             | PD_11           | 
E               | PD_13           |
PSB             | GND             | 
VCC             | 5 V             | 
GND             | GND             |
BLA             | 5 V             | 
BLK             | GND             |

### Firmware del sistema

#### Lógica y estructura

Aca va un diagrama en bloques de los módulos, main, sistema, subsistema, driver

Aca va un diagrama de los archivos de cada modulo

Aca va una descricpion y rol de cada modulo

Aca va un diagrama de flujo general del main

#### Modulos

Acá va una explicaion más detallada de los modulos y el diagrama de la maquina de estados si es que tiene, y una tabla con las funciones que lo componen explicando para que son y quienes las usan

## CAPITULO 4
### Ensayos y resultados

#### Pruebas funcionales de hardware
#### Pruebas de integracion con software
#### Cumplimiento de requisitos

## CAPITULO 5
### Conclusiones

#### Resultados obtenidos
#### Oportunidades de mejora



