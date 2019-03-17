package jp.araobp.ai.ftdi

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import jp.araobp.serial.Message
import jp.araobp.serial.jp.araobp.serial.service.FtdiDriver
import jp.araobp.serial.jp.araobp.serial.service.FtdiSimulator
import org.greenrobot.eventbus.EventBus
import org.greenrobot.eventbus.Subscribe
import org.greenrobot.eventbus.ThreadMode

class MainActivity : AppCompatActivity() {

    companion object {
        private val TAG = this::class.java.simpleName
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Register this activity with EventBus
        EventBus.getDefault().register(this)

        // Start SerialService
        //val intent = Intent(application, FtdiDriver::class.java)
        val intent = Intent(application, FtdiSimulator::class.java)
        startService(intent)
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    fun onMessage(message: Message) {
        Log.d(TAG, message.toString())
    }
}
