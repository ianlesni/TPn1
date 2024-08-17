# Trabajo final: MIDI Drum Pad v.1

### Alumno: Ian Lesnianski
## Objetivo: Definir los requisitos y casos de uso del trabajo final del curso online de sistemas embebidos

### 1. Selección del proyecto a implementar 

Siguiendo la filosofía de aprender desarrollando un proyecto que resulte de interés personal, se ha realizado una ponderación sobre los siguientes proyectos para definir cuál de los dos será el elegido en este trabajo final:

1. **Controlador MIDI**: Un dispositivo que permite controlar instrumentos y efectos virtuales de una estación de audio digital mediante mensajes MIDI, a través de pulsadores, encoders rotativos, y potenciómetros deslizables.
2. **MIDI Drum Pad**: Un dispositivo enfocado en la interpretación musical a través de pads sensibles al golpe, que envía mensajes MIDI a una estación de audio digital para generar sonidos. Incluye una interfaz de usuario de control y capacidad de almacenamiento de configuraciones.

Para poder comparar y evaluar de forma más profesional los proyectos, se han considerado los siguientes factores de selección:

Factor                     | Peso | Análisis del factor y criterios de puntuación sobre el proyecto                                                                                                                                  |
|--------------------------|------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------| 
Tiempo de implementación   | 10   | Evalúa cuánto tiempo se estima que llevará completar el proyecto. Una menor cantidad de tiempo necesario para la implementación recibe un puntaje más alto                                       |
Interés personal           | 10   | Representa el grado de interés y motivación personal en trabajar en el proyecto. A mayor interés, mayor es el puntaje                                                                            |
Aprendizaje asociado       | 9    | Mide el potencial de aprendizaje y adquisición de nuevas habilidades que ofrece el proyecto. A mayor aprendizaje, mayor es el puntaje                                                            |
Utilidad del proyecto      | 8    | Evalúa la relevancia y la potencial aplicación del proyecto en el mundo real. A mayor utilidad, mayor es el puntaje                                                                              |
Disponibilidad de Hardware | 6    | Refleja la facilidad con la que se puede acceder al hardware necesario para materializar el proyecto. Si el hardware es fácil de conseguir o ya se dispone del mismo, se asigna un puntaje mayor |

#### 🥇 MIDI Drum Pad  

MIDI Drum Pad   | Tiempo de implementación [10] | Interés personal [10] | Aprendizaje asociado [9] | Utilidad del proyecto [8] | Disponibilidad de Hardware [6] |
|---------------|-------------------------------|-----------------------|--------------------------|---------------------------|--------------------------------|
Puntuación      | 5                             | 10                    | 9                        | 10                        | 7                              |
Valor ponderado | 50                            | 100                   | 81                       |80                         | 42                             |  

**Puntaje total: 353 🥇**

Puntuación y justificaciones: 

* **Tiempo de implementación [5]**: 
Se asignó un puntaje medio debido a que, aunque se basa en el proyecto que se ha llevado a cabo en las anteriores entregas del presente curso, se prevé que la implementación llevará un tiempo considerable debido a la complejidad de manejar múltiples pads, las nuevas funcionalidades y la integración del sistema completo.
* **Interés personal [10]**:
El puntaje máximo se justifica considerando que el proyecto está perfectamente alineado con los intereses personales del desarrollador.
* **Aprendizaje asociado [9]**:
Dado que implica la integración y el procesamiento de múltiples pads, el manejo de los periféricos y la programación de un código mantenible y escalable, se le asigna un puntaje elevado.
* **Utilidad del proyecto [10]**:
La aplicación directa en la recreación musical hace que esté proyecto sea muy útil a los fines lúdicos del desarrollador.
* **Disponibilidad de Hardware [7]**:
Si bien la mayor parte del hardware necesario ya está disponible, aún queda pendiente adquirir elementos y resolver aspectos de diseño para sobrellevar problemáticas relacionadas a la propagación de los golpes sobre los pads aledaños.

#### 🥈 Controlador MIDI 

Controlador MIDI| Tiempo de implementación [10] | Interés personal [10] | Aprendizaje asociado [9] | Utilidad del proyecto [8] | Disponibilidad de Hardware [6] |
|---------------|-------------------------------|-----------------------|--------------------------|---------------------------|--------------------------------|
Puntuación      | 7                             | 9                     | 8                        | 7                         | 6                              |
Valor ponderado | 70                            | 90                    | 72                       | 56                        | 36                             |

**Puntaje total: 324 🥈**

* **Tiempo de implementación [7]**: 
Se estima que este proyecto podría implementarse en un tiempo razonable, pero sin embargo requiere un tiempo considerable para asegurar su funcionalidad.
* **Interés personal [9]**:
Se asigna una puntuación elevada acorde al interés por disponer de un dispositivo como este.
* **Aprendizaje asociado [8]**:
El proyecto ofrece una buena oportunidad para desarrollar una interfaz MIDI compleja y estudiar el protocolo en mayor profundidad. Pero, la implementación se vuelve repetitiva, dado que gran parte del trabajo implica la integración de múltiples componentes similares, como pulsadores, encoders y potenciómetros deslizables. Esto limita la variedad de los desafíos.
* **Utilidad del proyecto [7]**:
Si bien es un dispositivo útil, no representa una elevada utilidad para el desarrollador.
* **Disponibilidad de Hardware [6]**:
La puntuación se asigna considerando que sería necesario adquirir numerosos elementos.

Luego de analizar los factores y sus respectivas ponderaciones para cada uno de los proyectos, se concluye que el proyecto más adecuado para desarrollar es el **MIDI Drum Pad**. Además de presentar un desafío técnico significativo, este proyecto es de gran interés personal y se alinea mejor a la utilidad práctica que se busca obtener.

#### Descripción del proyecto

  El sistema consiste en un dispositivo que captura la información de una acción musical mediante transductores piezoeléctricos integrados en *drum pads*. Estos transductores convierten la deformación mecánica generada por un golpe en una señal eléctrica, la cual es procesada y transformada en un mensaje MIDI según el estándar MIDI. Estos mensajes se envían hacia una PC a través de una interfaz UART o mediante una conexión Bluetooth. Cualquiera sea el caso, la PC los recibe en el puerto COM. El software Hairless-MIDI (disponible en https://projectgus.github.io/hairless-midiserial/) interpreta los bytes de datos enviados desde la placa NUCLEO-429FZI como mensajes MIDI y los reenvía a un puerto MIDI de salida virtual, el cual pertenece al mismo software.
  
 Dado que el objetivo es que una Estación de Audio Digital (en este caso https://www.reaper.fm/) genere el sonido del instrumento, es necesario que esos mensajes MIDI, interpretados por el software Hairless-MIDI, sean transmitidos a dicha estación. Aunque tanto Hairless-MIDI como Reaper pueden comprender y procesar mensajes MIDI, no poseen una forma nativa de establecer una conexión directa entre sí. Para solucionar esta limitación, se emplea el software loopMIDI (disponible en https://www.tobias-erichsen.de/software/loopmidi.html) que crea un canal virtual llamado "loopMIDI Port" permitiendo la comunicación entre ambos programas a través mensajes MIDI.

 Además, el dispositivo contará con una interfaz de control de usuario, compuesta por un display LCD gráfico que facilitará la visualización de las configuraciones y los distintos menús del sistema. Esta interfaz también incluirá un encoder rotativo y dos pulsadores, que permitirán al usuario desplazarse por las opciones de configuración y confirmar o retroceder en los menús, mejorando la usabilidad e interacción con el sistema.
 Adicionalmente, se incorporará una memoria micro-SD para almacenar las distintas configuraciones del dispositivo. Esto permitirá guardar varios drum kits con pads asociados a diferentes instrumentos, lo que ofrecerá una mayor flexibilidad en la personalización del instrumento.

 Por último, el desarrollo también incluirá un pedal de control de hi-hat, permitiendo un mayor realismo en la ejecución del pedal de hi-hat, al emular de manera más precisa el comportamiento de un hi-hat tradicional.

#### Diagrama en bloques:

![Diagrama en bloques TP final](https://github.com/user-attachments/assets/00c6026d-d886-4869-9de9-9d541defa3af)


### 2. Elicitación de requisitos y casos de uso
Para establecer un punto de referencia, se compararon características de productos similares de marcas líderes en el mercado, como Alesis, Roland y Yamaha. Es importante mencionar que este desarrollo no busca superar las prestaciones de los productos listados a continuación, si no tomar sus características como referencias.
El proyecto pone el foco en ser una versión económica que tenga las prestaciónes básicas de este tipo de productos.

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

#### Requisitos

Grupo de requisito     | ID  | Descripción                                                                                                                                                                                          |
-----------------------|-----|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|   
1.Sensado              | 1.1 | El sistema tendrá sensibilidad a los golpes sobre los 4 drum pads, detectando la intensidad de los golpes con una resolución de 12 bits                                                              |
  ---                  | 1.2 | El sistema contará con un pedal de control de hi-hat, cuya posición será leída con una resolución de 12 bits                                                                                         |
2.Configuraciones      | 2.1 | Podrá modificarse la nota MIDI asociada a cada uno de los drum pads de forma individual                                                                                                              |
  ---                  | 2.2 | Podrá modificarse la sensibilidad al golpe de cada uno de los drum pads de forma individual                                                                                                          |
  ---                  | 2.3 | Podrá modificarse la reverberación de cada uno de los drum pads de forma individual                                                                                                                  |
  ---                  | 2.4 | Podrán configurarse 10 drum kits                                                                                                                                                                     |
  ---                  | 2.5 | Podrán configurarse los canales MIDI de cada drum kit                                                                                                                                                |
  ---                  | 2.6 | Podrá configurarse el volumen general de cada drum kit                                                                                                                                               |
3.Interfaz de control  | 3.1 | El sistema dispondrá de un display LCD gráfico para mostrar las configuraciones y menús                                                                                                              |
  ---                  | 3.2 | El sistema contará con dos pulsadores: uno para confirmar las selecciones del menú y otro para cancelarlas y/o volver a la opción anterior                                                           | 
  ---                  | 3.3 | El sistema contará con un encoder rotativo para navegar por las configuraciones y menús                                                                                                              |
  ---                  | 3.3 | El sistema contará con un led en cada pad que se ilumina cada vez que el pad recibe un golpe                                                                                                         |
4.Conectividad         | 4.1 | El sistema podrá conectarse a la PC y enviar los mensajes MIDI mediante un cable USB, a través del conversor USB-UART de la placa Nucleo                                                             |
  ---                  | 4.2 | El sistema podrá conectarse a la PC y enviar los mensajes MIDI mediante conexión Bluetooth                                                                                                           |
5.Almacenamiento       | 5.1 | El sistema permitirá guardar configuraciones y drum kits en una tarjeta microSD de 2GB                                                                                                               |
6.Alimentación         | 6.1 | El sistema se podrá alimentarse desde el puerto USB de la PC                                                                                                                                         |
  ---                  | 6.2 | El sistema se podrá alimentarse desde un cargador con salida USB de 5V 500mA(o superior)                                                                                                             |
7.Tiempo de desarrollo | 7.1 | El proyecto se entregará el día 17 de septiembre de 2024                                                                                                                                             |   
8.Documentación        | 8.1 | Se entregará el código del proyecto en un repositorio de GitHub, una lista de partes, un diagrama de conexiones detallada, y una nota de oportunidades de mejora con un balance de los resultados obtenidos | 

#### Casos de uso

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


