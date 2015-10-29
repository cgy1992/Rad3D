package com.myway;

import android.content.Context;
import android.text.Selection;
import android.text.Spannable;
import android.view.KeyEvent;
import android.widget.EditText;


public class MGLEditText  extends EditText
{
	private MGLView mView;
	
	MGLEditText(final Context context)
	{
		super(context);
	}
	
	public void SetGLView(MGLView view)
	{
		mView = view;
	}
	
	public MGLView GetGLView()
	{
		return this.mView;
	}
	
	public void InitForEdit()
	{
		setText(" ");
		
		CharSequence text = getText();
	
		if (text instanceof Spannable) {
			Spannable spanText = (Spannable)text;
			Selection.setSelection(spanText, 1);
		}
	}
	
	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent)
	{
		super.onKeyDown(pKeyCode, pKeyEvent);

		if (pKeyCode == KeyEvent.KEYCODE_BACK)
		{
			mView.requestFocus();
		}

		return true;
	}
}





