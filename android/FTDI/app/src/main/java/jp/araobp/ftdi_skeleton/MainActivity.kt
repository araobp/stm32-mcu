package jp.araobp.ftdi_skeleton

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.widget.TextView
import jp.araobp.serial.MessageFromDevice
import jp.araobp.serial.MessageToDevice
import jp.araobp.serial.service.FtdiSimulator
import org.greenrobot.eventbus.EventBus
import org.greenrobot.eventbus.Subscribe
import org.greenrobot.eventbus.ThreadMode

class MainActivity : AppCompatActivity() {

    companion object {
        private val TAG = this::class.java.simpleName
    }

    private var mTextView: TextView? = null

    private val mEventBus = EventBus.getDefault()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Register this activity with EventBus
        EventBus.getDefault().register(this)

        // Start SerialService
        //val intent = Intent(application, FtdiDriver::class.java)
        val intent = Intent(application, FtdiSimulator::class.java)
        startService(intent)

        setContentView(R.layout.activity_main)
        mTextView = findViewById(R.id.textView)
        mTextView!!.movementMethod = ScrollingMovementMethod()
    }

    override fun onDestroy() {
        super.onDestroy()
        EventBus.getDefault().unregister(this)
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    fun onMessage(message: MessageFromDevice) {
        //Log.d(TAG, message.toString())
        mTextView!!.append(message.message + "\n")
        var messageToDevice = MessageToDevice("Hello![%s]".format(message.toString()))
        mEventBus.post(messageToDevice)
    }
}
