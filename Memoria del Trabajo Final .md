## Memoria de Trabajo Final:
# MIDI Drum Kit

## Autor:
# Ing. Ian Lesnianski

# Resumen
  Este trabajo presenta el desarrollo de un sistema electrónico que captura acciones musicales mediante drum pads con transductores piezoeléctricos, los cuales convierten los golpes en señales eléctricas que se procesan y transforman en mensajes MIDI. Estos mensajes se transmiten a una computadora vía UART o Bluetooth, donde se interpretan con el software Hairless-MIDI y se envían a una Estación de Audio Digital (DAW), como Reaper, para generar el sonido del instrumento.
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

![Diagrama en bloques](https://github.com/user-attachments/assets/f28c5ccb-20cd-4d31-93cf-401ee9897b42)

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

Grupo de requisito     | ID  | Descripción                                                                                                                                                                                          |
-----------------------|-----|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|   
1.Sensado              | 1.1 | El sistema tendrá sensibilidad a los golpes sobre los 3 drum pads, detectando la intensidad de los golpes con una resolución de 12 bits                                                              |
  ---                  | 1.2 | El sistema contará con un pedal de control de hi-hat, cuya posición será leída con una resolución de 12 bits                                                                                         |
2.Configuraciones      | 2.1 | Podrá modificarse la nota MIDI asociada a cada uno de los drum pads de forma individual                                                                                                              |
  ---                  | 2.2 | Podrá modificarse la sensibilidad al golpe de cada uno de los drum pads de forma individual                                                                                                          |
  ---                  | 2.3 | Podrá modificarse la reverberación de cada uno de los drum pads de forma individual                                                                                                                  |
  ---                  | 2.4 | Podrán configurarse los canales MIDI de cada drum kit                                                                                                                                                                   |
  ---                  | 2.5 |   Podrá configurarse el volumen general de cada drum kit                                                                                                                                                                                                                             |
3.Interfaz de control  | 3.1 | El sistema dispondrá de un display LCD gráfico para mostrar las configuraciones y menús                                                                                                              |
  ---                  | 3.2 | El sistema contará con dos pulsadores: uno para confirmar las selecciones del menú y otro para cancelarlas y/o volver a la opción anterior                                                           | 
  ---                  | 3.3 | El sistema contará con un encoder rotativo para navegar por las configuraciones y menús                                                                                                              |
  ---                  | 3.3 | El sistema contará con un led en cada drum pad que se ilumina cada vez que recibe un golpe                                                                                                         |
4.Conectividad         | 4.1 | El sistema podrá conectarse a la PC y enviar los mensajes MIDI mediante un cable USB, a través del conversor USB-UART de la placa Nucleo                                                             |
  ---                  | 4.2 | El sistema podrá conectarse a la PC y enviar los mensajes MIDI mediante conexión Bluetooth                                                                                                           |
                                                                                                          |
5.Alimentación         | 5.1 | El sistema se podrá alimentarse desde el puerto USB de la PC                                                                                                                                         |
  ---                  | 5.2 | El sistema se podrá alimentarse desde un cargador con salida USB de 5V 500mA(o superior)                                                                                                             |
6.Tiempo de desarrollo | 6.1 | El proyecto se entregará el día 17 de septiembre de 2024                                                                                                                                             |   
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

Elemento del caso de uso | Definición                                                                                                                            |
-------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
N°                       | 3  
Título                   | Conexión Bluetooth con la PC                                                                                                                                      |
Disparador               | El usuario habilita el Bluetooth del sistema                                                                                                                                      |
Precondición             | El sistema está encendido, en el menú de configuración general                                                                                                                                      |
Flujo básico             | El usuario selecciona la opción "Bluetooth en el menú. El usuario busca desde la PC al "MIDI Drum pad" en la sección de dispositivos Bluetooth y lo empareja. Se establece la conexión y el sistema comienza a enviar los mensajes MIDI vía Bluetooth |                                                                                                                                      |
Flujo alternativo        | 3.a La conexión falla. El sistema muestra en su display un mensaje de error y regresa al menú principal                                                                                                                             |

### Descripción del hardware empleado

En esta sección se plasmaran los detalles de los módulos de hardware que componen al trabajo final:
- 1: Transductor piezoeléctirco
- 2:
- 3:
- 4:
- 5:

### Introducción al protocolo MIDI

# CAPÍTULO 3
## Diseño e implementación




