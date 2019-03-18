package jp.araobp.ai.ftdi

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.util.Log
import android.widget.TextView
import jp.araobp.serial.Message
import jp.araobp.serial.service.FtdiDriver
import org.greenrobot.eventbus.EventBus
import org.greenrobot.eventbus.Subscribe
import org.greenrobot.eventbus.ThreadMode

class MainActivity : AppCompatActivity() {

    companion object {
        private val TAG = this::class.java.simpleName
    }

    private var mTextView: TextView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Register this activity with EventBus
        EventBus.getDefault().register(this)

        // Start SerialService
        val intent = Intent(application, FtdiDriver::class.java)
        //val intent = Intent(application, FtdiSimulator::class.java)
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
    fun onMessage(message: Message) {
        //Log.d(TAG, message.toString())
        mTextView!!.append(message.message + "\n")
    }
}
