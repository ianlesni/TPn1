### Memoria de Trabajo Final:
# MIDI Drum Kit

### Autor
# Ing. Ian Lesnianski

# Resumen
  El proyecto consiste en el desarrollo de un sistema electrónico capaz de capturar acciones musicales mediante transductores piezoeléctricos integrados en drum pads. Estos transductores convierten la energía mecánica generada por golpes en señales eléctricas, las cuales son procesadas y convertidas en mensajes MIDI, siguiendo el estándar de este protocolo. Los mensajes MIDI se envían a una computadora a través de una interfaz UART o Bluetooth, permitiendo que un software interprete estos mensajes como eventos musicales.

Para que una Estación de Audio Digital, como Reaper, genere sonidos a partir de los eventos MIDI, se emplea el software Hairless-MIDI para interpretar los mensajes recibidos y loopMIDI para establecer una conexión virtual entre ambos programas. Esto permite que el sistema funcione como un instrumento musical digital, donde la información capturada por los drum pads es procesada y transmitida para ser reproducida en tiempo real.

El sistema incluye una interfaz de usuario para la configuración de atributos del drumkit, tres pads sensibles a la intensidad de los golpes configurables y un pedal de control para el hi-hat. Este trabajo integra hardware y software para la creación de un dispositivo funcional orientado a la producción musical.



