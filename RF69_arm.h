#include <SPI.h>
#include <util/atomic.h>

#define ROM_UINT8       const uint8_t
#define ROM_READ_UINT8  *
#define ROM_DATA        

#define IRQ_ENABLE      __enable_irq()

static SPISettings settings(4000000, MSBFIRST, SPI_MODE0);

#if defined(TEENSYDUINO) && defined(KINETISL) // Teensy LC

#define CS_PIN 10

static void spiInit (void) {
    digitalWriteFast(CS_PIN, HIGH);
    pinMode(CS_PIN, OUTPUT);
    digitalWriteFast(CS_PIN, HIGH);
    SPI.setSCK(14);
    SPI.begin();
}

#endif

class PreventInterrupt {
    private:
        uint32_t primask;
    public:
        PreventInterrupt () { this->primask = __get_primask(); __disable_irq(); }
        ~PreventInterrupt () { if (!this->primask) __enable_irq(); }
};

static uint8_t spiTransfer (uint8_t cmd, uint8_t val) {
    SPI.beginTransaction(settings);
    digitalWriteFast(CS_PIN, LOW);
    SPI.transfer(cmd);
    uint8_t rec = SPI.transfer(val);
    digitalWriteFast(CS_PIN, HIGH);
    SPI.endTransaction();
    return rec;
}

