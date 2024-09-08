package com.AndroidCompat;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import java.io.File;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class AndroidCompat extends Activity
{
    private void UnpackAssets()
    {

    }

    /** Se llama cuando la actividad se crea por primera vez. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Cree un objeto TextView y establezca el texto en "Hola a todos". */
        TextView  tv = new TextView(this);
        tv.setText("Hello World!");
        setContentView(tv);
    }
}
