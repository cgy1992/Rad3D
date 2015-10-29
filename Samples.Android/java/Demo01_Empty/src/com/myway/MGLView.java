package com.myway;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.inputmethod.InputMethodManager;

class MGLHandler extends Handler
{
	@Override
	public void handleMessage(final Message msg)
	{	
		if (msg.what == MGLView.HANDLER_OPEN_KEYBOARD)
		{
			if (MGLView.msInstance != null && MGLView.msInstance.mGLEditText.requestFocus())
			{
				MGLView.msInstance.mGLEditText.removeTextChangedListener(MGLView.msInstance.mGLTextWatcher);
				{
					MGLView.msInstance.mGLEditText.InitForEdit();
				}
				MGLView.msInstance.mGLEditText.addTextChangedListener(MGLView.msInstance.mGLTextWatcher);
				 
				InputMethodManager imm = (InputMethodManager)MGLView.msInstance.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				imm.showSoftInput(MGLView.msInstance.mGLEditText, 0);
			}
		}
		else if (msg.what == MGLView.HANDLER_CLOSE_KEYBOARD)
		{
			if (MGLView.msInstance != null)
			{
				MGLView.msInstance.mGLEditText.removeTextChangedListener(MGLView.msInstance.mGLTextWatcher);
				
				InputMethodManager imm = (InputMethodManager)MGLView.msInstance.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				imm.hideSoftInputFromWindow(MGLView.msInstance.mGLEditText.getWindowToken(), 0);
			}
		}
	}
}


public class MGLView extends GLSurfaceView {
	
	protected MGLRenderer mGLRenderer;
	protected MGLEditText mGLEditText;
	protected MGLTextWatcher mGLTextWatcher;
	
	public static MGLView msInstance = null;
	
	private static Handler msHandler = null;
	public final static int HANDLER_OPEN_KEYBOARD = 2;
	public final static int HANDLER_CLOSE_KEYBOARD = 3;
	
	
	public MGLView(Context context)
	{
        super(context);
        msInstance = this;
        
        init();
    }
	
	public MGLView(Context context, AttributeSet attri)
	{
		super(context, attri);
		msInstance = this;
		
		init();
	}
	
	private void init()
	{
		this.setFocusableInTouchMode(true);
		this.setEGLContextClientVersion(2);
		this.setEGLConfigChooser(5, 6, 5, 0, 24, 0);
		
		msHandler = new MGLHandler();
	}
	
	public void SetGLRenderer(MGLRenderer rd)
	{
		mGLRenderer = rd;
		this.setRenderer(rd);
	}
	
	public void SetGLEditText(MGLEditText et)
	{
		mGLEditText = et;
		mGLEditText.SetGLView(this);
		this.requestFocus();
	}
	
	public void SetGLTextWatcher(MGLTextWatcher tw)
	{
		mGLTextWatcher = tw;
	}
	
	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onResume()
	{
		super.onResume();
		
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				mGLRenderer.onResume();
			}
		});
	}

	@Override
	public void onPause()
	{
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				mGLRenderer.onPause();
			}
		});
		
		super.onPause();
	}
	
	public void onDestroy()
	{
		mGLRenderer.onDestroy();
	}
	
	public static void OpenKeyboard()
	{
		final Message msg = new Message();
		msg.what = HANDLER_OPEN_KEYBOARD;
		msHandler.sendMessage(msg);
	}
	
	public static void CloseKeyboard()
	{
		final Message msg = new Message();
		msg.what = HANDLER_CLOSE_KEYBOARD;
		msHandler.sendMessage(msg);
	}
	
	@Override
	public boolean onTouchEvent(final MotionEvent pMotionEvent)
	{
		int flag = pMotionEvent.getAction() & MotionEvent.ACTION_MASK;
		
		if (flag == MotionEvent.ACTION_POINTER_DOWN)
		{
			this.queueEvent(new Runnable() {
				@Override
				public void run()
				{
					final int i = pMotionEvent.getActionIndex();
					final int id = pMotionEvent.getPointerId(i);
					final float x = pMotionEvent.getX(i);
					final float y = pMotionEvent.getY(i);
					
					mGLRenderer.onActionDown(id, x, y);
				}
			});
		}
		else if (flag == MotionEvent.ACTION_POINTER_UP)
		{
			this.queueEvent(new Runnable() {
				@Override
				public void run()
				{
					final int i = pMotionEvent.getActionIndex();
					final int id = pMotionEvent.getPointerId(i);
					final float x = pMotionEvent.getX(i);
					final float y = pMotionEvent.getY(i);
					
					mGLRenderer.onActionUp(id, x, y);
				}
			});
		}
		else if (flag == MotionEvent.ACTION_DOWN)
		{
			this.queueEvent(new Runnable() {
				@Override
				public void run()
				{
					final int i = 0;
					final int id = pMotionEvent.getPointerId(i);
					final float x = pMotionEvent.getX(i);
					final float y = pMotionEvent.getY(i);
					
					mGLRenderer.onActionDown(id, x, y);
				}
			});
		}
		else if (flag == MotionEvent.ACTION_UP)
		{
			this.queueEvent(new Runnable() {
				@Override
				public void run()
				{
					final int i = 0;
					final int id = pMotionEvent.getPointerId(i);
					final float x = pMotionEvent.getX(i);
					final float y = pMotionEvent.getY(i);
					
					mGLRenderer.onActionUp(id, x, y);
				}
			});
		}
		else if (flag == MotionEvent.ACTION_MOVE)
		{
			this.queueEvent(new Runnable() {
				@Override
				public void run()
				{
					for (int i = 0; i < pMotionEvent.getPointerCount(); ++i)
					{
						final int id = pMotionEvent.getPointerId(i);
						final float x = pMotionEvent.getX(i);
						final float y = pMotionEvent.getY(i);
						
						mGLRenderer.onActionMove(id, x, y);
					}
				}
			});
		}
		else if (flag == MotionEvent.ACTION_CANCEL)
		{
			this.queueEvent(new Runnable() {
				@Override
				public void run()
				{
					for (int i = 0; i < pMotionEvent.getPointerCount(); ++i)
					{
						final int id = pMotionEvent.getPointerId(i);
						final float x = pMotionEvent.getX(i);
						final float y = pMotionEvent.getY(i);
						
						mGLRenderer.onActionCancel(id, x, y);
					}
				}
			});
		}
		
		return true;
	}
	
	public void insertText(final String text)
	{
		this.queueEvent(new Runnable() {
			@Override
			public void run()
			{
				mGLRenderer.onInsertString(text);
			}
		});
	}
	
	public void deleteBackward()
	{
		this.queueEvent(new Runnable() {
			@Override
			public void run()
			{
				mGLRenderer.onDeleteBackword();
			}
		});
	}
	
}

