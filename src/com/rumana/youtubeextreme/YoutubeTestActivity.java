package com.rumana.youtubeextreme;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class YoutubeTestActivity extends Activity {
	//private static final int PLAY_ID = Menu.FIRST;
	
	  @Override
	  protected void onCreate(Bundle pSavedInstanceState) {
	    super.onCreate(pSavedInstanceState);
	    
	    setContentView(R.layout.main);
	    
	    final TextView videoIdTextView = (TextView) findViewById(R.id.youtubeIdText);
	    final Button   viewVideoButton = (Button)   findViewById(R.id.viewVideoButton);
	    final TextView proxyipTextView     = (TextView) findViewById(R.id.proxyiptext);
	    final TextView proxyportTextView     = (TextView) findViewById(R.id.proxyporttext);
	    
	    viewVideoButton.setOnClickListener(new View.OnClickListener() {
	      
	      @Override
	      public void onClick(View pV) {

	    	  // TODO handle exceptions
	        String videoId = videoIdTextView.getText().toString();
	        String proxyIp = proxyipTextView.getText().toString();
	        String proxyPort  = proxyportTextView.getText().toString();
	        
	        if(videoId == null || videoId.trim().equals("")){
	          videoId = "EE8L2VNf6sw";
	        }
	        if(proxyIp == null) proxyIp = "";
	        if(proxyPort == null) proxyPort = "0";
	        
	        invokeYoutubeActivity(videoId, proxyIp, Integer.parseInt(proxyPort));
	        
	      }
	    });
	    
	  }
	  
	    private void invokeYoutubeActivity(String videoId, String proxyIp, int proxyPort) {
	    	Bundle bundle = new Bundle();
	    	bundle.putString("ProxyIP", proxyIp);
	    	bundle.putInt("ProxyPort", proxyPort);
	        Intent lVideoIntent = new Intent(null, Uri.parse("ytv://"+videoId), YoutubeTestActivity.this, OpenYouTubePlayerActivity.class);
	        lVideoIntent.putExtras(bundle);
	        startActivity(lVideoIntent);
	    }
	
/*	private LocalService mBoundService;

	private ServiceConnection mConnection = new ServiceConnection() {
	    public void onServiceConnected(ComponentName className, IBinder service) {
	        // This is called when the connection with the service has been
	        // established, giving us the service object we can use to
	        // interact with the service.  Because we have bound to a explicit
	        // service that we know is running in our own process, we can
	        // cast its IBinder to a concrete class and directly access it.
	        mBoundService = ((LocalService.LocalBinder)service).getService();

	        // Tell the user about this for our demo.
	        Toast.makeText(Binding.this, R.string.local_service_connected,
	                Toast.LENGTH_SHORT).show();
	    }

	    public void onServiceDisconnected(ComponentName className) {
	        // This is called when the connection with the service has been
	        // unexpectedly disconnected -- that is, its process crashed.
	        // Because it is running in our same process, we should never
	        // see this happen.
	        mBoundService = null;
	        Toast.makeText(Binding.this, R.string.local_service_disconnected,
	                Toast.LENGTH_SHORT).show();
	    }
	};

	void doBindService() {
	    // Establish a connection with the service.  We use an explicit
	    // class name because we want a specific service implementation that
	    // we know will be running in our own process (and thus won't be
	    // supporting component replacement by other applications).
	    bindService(new Intent(Binding.this, 
	            LocalService.class), mConnection, Context.BIND_AUTO_CREATE);
	    mIsBound = true;
	}

	void doUnbindService() {
	    if (mIsBound) {
	        // Detach our existing connection.
	        unbindService(mConnection);
	        mIsBound = false;
	    }
	}

	@Override
	protected void onDestroy() {
	    super.onDestroy();
	    doUnbindService();
	}*/
}