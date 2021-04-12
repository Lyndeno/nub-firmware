package com.example.nubapplication;

import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.StrictMode;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private Button connect_button;
    private TextView SenderMAC;

    Boolean turn;
    static WifiManager wifiManager;
    Context context;
    WifiConfiguration conf;
    public static String networkSSID = "NUB";
    public static String networkPass = "capstone1234";
    byte[] buf = new byte[1024];


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SenderMAC = (TextView) findViewById(R.id.sender_mac);
        connect_button = (Button) findViewById(R.id.connect_button);


        String mac_address = getMacAddress();
        SenderMAC.setText("\n   YOUR MAC ADDRESS : " + mac_address);


        connect_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openTextingActivity();
                wifi_connect();
            }
        });

        turn= true;
        context = this;
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

    }




    public void openTextingActivity() {
        android.content.Intent intent = new Intent(this, TextingActivity.class);
        startActivity(intent);
    }


// Wifi Connect/Disconnect Activity
    public void wifi_connect() {

        wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);

        if (turn) {

            turnOnOffWifi(context, true);
            turn = false;
            Toast.makeText(getApplicationContext(), "turning on...", Toast.LENGTH_SHORT).show();

            conf = new WifiConfiguration();
            conf.SSID = "\"" + networkSSID + "\"";
            conf.preSharedKey = "\"" + networkPass + "\"";
            conf.status = WifiConfiguration.Status.ENABLED;
            conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
            conf.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
            conf.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
            conf.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
            int netid = wifiManager.addNetwork(conf);
            wifiManager.disconnect();
            wifiManager.enableNetwork(netid, true);
            wifiManager.reconnect();

        }
        else {
            turnOnOffWifi(context, false);
            turn = true;
            Toast.makeText(getApplicationContext(), "turning off...", Toast.LENGTH_SHORT).show();
        }

    }

    public static void turnOnOffWifi(Context context, boolean isTurnToOn) {
        wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(isTurnToOn);
    }



    // Getting MAC address of device
    public String getMacAddress(){
        try{
            List<NetworkInterface> networkInterfaceList = Collections.list(NetworkInterface.getNetworkInterfaces());

            String stringMac = "";

            for(java.net.NetworkInterface networkInterface : networkInterfaceList)
            {
                if(networkInterface.getName().equalsIgnoreCase("wlon0"));
                {
                    for(int i = 0 ;i <networkInterface.getHardwareAddress().length; i++){
                        String stringMacByte = Integer.toHexString(networkInterface.getHardwareAddress()[i]& 0xFF);

                        if(stringMacByte.length() == 1)
                        {
                            stringMacByte = "0" +stringMacByte;
                        }

                        stringMac = stringMac + stringMacByte.toUpperCase() + ":";
                    }
                    break;
                }

            }
            return stringMac;
        }
        catch (SocketException e)
        {
            e.printStackTrace();
        }

        return  "0";
    }

}
