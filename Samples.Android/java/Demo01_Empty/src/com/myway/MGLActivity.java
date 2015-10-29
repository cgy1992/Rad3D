package com.myway;

import android.os.Bundle;
import android.app.Activity;
import android.view.ViewGroup;
import android.widget.FrameLayout;

//import android.view.Window;
//import android.view.WindowManager;


public class MGLActivity extends Activity
{
	protected MGLView mView;
	
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        onCreateView();
    }

    protected void onCreateView()
    {
    	// Frame Layout
    	//
		@SuppressWarnings("deprecation")
		ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                       ViewGroup.LayoutParams.FILL_PARENT);
        FrameLayout framelayout = new FrameLayout(this);
        framelayout.setLayoutParams(framelayout_params);
        
        // Edit Text (Helper of Open Keyboard)
        //
        @SuppressWarnings("deprecation")
        ViewGroup.LayoutParams edittext_layout_params =
                new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                           ViewGroup.LayoutParams.WRAP_CONTENT);
        
        MGLEditText editText = new MGLEditText(this);
        editText.setLayoutParams(edittext_layout_params);
        framelayout.addView(editText);
        
        // GL View
        //
        mView = new MGLView(this);
        framelayout.addView(mView);
        
        @SuppressWarnings("deprecation")
        int screenWidth = this.getWindowManager().getDefaultDisplay().getWidth();
        @SuppressWarnings("deprecation")
        int screenHeight = this.getWindowManager().getDefaultDisplay().getHeight();

        mView.SetGLRenderer(new MGLRenderer(this, screenWidth, screenHeight));
        mView.SetGLEditText(editText);
        mView.SetGLTextWatcher(new MGLTextWatcher(editText));

		setContentView(framelayout);
    }
    
    @Override
    protected void onDestroy()
    {
    	mView.onDestroy();
    	super.onDestroy();
    }
    
    @Override
    protected void onStop()
    {
    	super.onStop();
    }

    @Override
    protected void onResume()
    {
    	super.onResume();
    	mView.onResume();
    }
    
    @Override
    protected void onPause()
    {
    	mView.onPause();
    	super.onPause();
    }
    
    
    
}
