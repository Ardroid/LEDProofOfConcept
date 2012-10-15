package arduino.concept;

import java.io.IOException;

import org.microbridge.server.Client;
import org.microbridge.server.Server;
import org.microbridge.server.ServerListener;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class LEDControllerProofOfConceptActivity extends Activity implements
		OnClickListener, ServerListener {
	private static final byte CONNECTION_NOT_ESTABLISHED = 0;
	private static final byte CONNECTION_ESTABLISHED = 1;
	private static final byte BOT_IDLING = 2;
	private static final byte BOT_MOVING = 3;

	private Server m_server = null;
	private Client m_client = null;
	private boolean connected = false;

	private Handler m_handler = new Handler();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		m_server = new Server(4567);
		try {
			m_server.start();
		} catch (IOException ex) {
			Log.wtf("ardroid", "Unable to start TCP server!");
			System.exit(-1);
		}
		m_server.addListener(this);
		Button notEstablishedButton = (Button) findViewById(R.id.established);
		Button establishedButton = (Button) findViewById(R.id.not_established);
		Button idleButton = (Button) findViewById(R.id.idling);
		Button movingButton = (Button) findViewById(R.id.moving);

		notEstablishedButton.setOnClickListener(this);
		establishedButton.setOnClickListener(this);
		idleButton.setOnClickListener(this);
		movingButton.setOnClickListener(this);
	}

	private void setStatus(String statusText) {
		TextView status = (TextView) findViewById(R.id.statusTextView);
		status.setText(statusText);
	}

	private void setStatus(int resid) {
		TextView status = (TextView) findViewById(R.id.statusTextView);
		status.setText(resid);
	}

	@Override
	public void onClick(View v) {
		byte[] data = new byte[1];
		switch (v.getId()) {
			case R.id.not_established:
				data[0] = CONNECTION_NOT_ESTABLISHED;
			break;

			case R.id.established:
				data[0] = CONNECTION_ESTABLISHED;
			break;

			case R.id.idling:
				data[0] = BOT_IDLING;
			break;

			case R.id.moving:
				data[0] = BOT_MOVING;
			break;
		}
		boolean error = false;
		try {
			m_server.send(data);
		} catch (IOException ex) {
			error = true;
		} finally {
			final int s = error ? R.string.error
					: (connected ? R.string.connected : R.string.not_connected);
			m_handler.post(new Runnable() {
				public void run() {
					setStatus(s);
				}
			});
		}
	}

	@Override
	public void onServerStarted(Server server) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onServerStopped(Server server) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onClientConnect(Server server, Client client) {
		connected = true;
		m_handler.post(new Runnable() {
			public void run() {
				setStatus(R.string.connected);
			}
		});

	}

	@Override
	public void onClientDisconnect(Server server, Client client) {
		connected = false;
		m_handler.post(new Runnable() {
			public void run() {
				setStatus(R.string.not_connected);
			}
		});
	}

	@Override
	public void onReceive(Client client, byte[] data) {
		// TODO Auto-generated method stub

	}
}