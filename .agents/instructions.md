# Directrices de Desarrollo para Sistemas Embebidos (UCAB)
- Arquitectura objetivo: ESP32 dual-core Xtensa LX6.
- Sistema Operativo: FreeRTOS.
- Prohibiciones estrictas:
  - No utilizar delay(). Usar siempre vTaskDelay() o temporizadores por hardware.
  - Prohibido el uso de variables globales compartidas entre tareas sin mecanismos de sincronización (Mutex, Semáforos o Colas).
  - No instanciar objetos dinámicos pesados dentro de bucles continuos.
- Prácticas requeridas:
  - Gestionar estados mediante enumeraciones y estructuras switch-case sin bloqueos.
