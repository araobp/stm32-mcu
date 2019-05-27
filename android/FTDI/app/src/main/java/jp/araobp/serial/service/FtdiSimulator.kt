package jp.araobp.serial.service

import android.util.Log
import jp.araobp.serial.MessageFromDevice
import jp.araobp.serial.MessageToDevice

import jp.araobp.serial.SerialService
import org.greenrobot.eventbus.EventBus
import org.greenrobot.eventbus.Subscribe
import org.greenrobot.eventbus.ThreadMode

/**
 * FTDI device driver
 *
 */
class FtdiSimulator : SerialService() {

    companion object {
        const val SLEEP = 1000L  // 1sec interval
    }

    /**
     * Sets FTDI device config
     */
    private fun setConfig(baudrate: Int, dataBits: Byte, stopBits: Byte, parity: Byte, flowControl: Byte) {
        Log.d("setConfig", "$baudrate, $dataBits, $stopBits, $parity, $flowControl")
    }

    // reader thread
    private val mReader = Runnable {
        var sno = 0  // Serial number
        while (true) {
            sno += 1  // Increment the serial number
            var data = "sno=%d\n".format(sno) // Data received from a device
            var offset = data.length
            var mCharBuf = data.toCharArray()
            rx(String(mCharBuf, 0, offset - 1))
            Thread.sleep(SLEEP)
        }
    }

    /**
     * Opens FTDI device and start reader thread
     *
     * @parameter baudrate baud rate
     * @return true if FTDI device is opened successfully
     */
    override fun open(baudrate: Int): Boolean {
        setConfig(baudrate, 8.toByte(), 1.toByte(), 0.toByte(), 0.toByte())
        Thread(mReader).start()
        return true
    }

    /**
     * Transmits a message to FTDI device
     */
    override fun tx(message: String) {
        Log.d("tx", message)
    }

    /**
     * Stops reader thread and closes FTDI device
     */
    override fun close() {
    }
}