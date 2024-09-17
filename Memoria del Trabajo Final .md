### Memoria de Trabajo Final:
# MIDI Drum Kit

### Autor
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

El desarrollo de este sistema no solo involucró la captura y procesamiento de señales musicales, sino también la implementación de una interfaz de usuario para facilitar la configuración de distintos atributos del drumkit. Esta interfaz incluyó un display gráfico LCD, un encoder rotativo y dos pulsadores, que permiten al usuario navegar por los menús del sistema y ajustar las configuraciones según sus necesidades. Asimismo, se añadió un pedal de control de hi-hat, mejorando la capacidad del dispositivo para emular el comportamiento de un set de batería tradicional, brindando una experiencia más realista en la ejecución musical.

Este trabajo se destaca por integrar múltiples tecnologías y protocolos, lo que lo diferencia de otros sistemas similares en su capacidad de comunicación flexible, su interfaz de usuario avanzada, y la incorporación de elementos que mejoran la usabilidad del dispositivo. En el Capítulo 3, se presentarán en detalle los aspectos técnicos de la implementación, incluyendo el hardware y software utilizado, mientras que en el Capítulo 4 se discutirán los resultados obtenidos y las pruebas realizadas




