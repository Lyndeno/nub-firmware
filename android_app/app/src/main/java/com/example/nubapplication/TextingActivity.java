package com.example.nubapplication;

import androidx.appcompat.app.AppCompatActivity;


import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.net.InetAddress;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class TextingActivity extends AppCompatActivity implements View.OnClickListener {

    // Initializing Variables

    private Button disconnect_button;
    private TextView textResponse;
    private EditText SenderMac;
    private EditText Message;
    private EditText Recipient;
    private Button sendbutton;
    String IpAddress = "192.168.4.1";//Ip address of esp
    int Port = 3333;//Wifi Server Number to be filled out;

    Boolean turn;
    static WifiManager wifiManager;
    Context context;
    WifiConfiguration conf;
    public static String networkSSID = "NUB";
    public static String networkPass = "capstone1234";




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_texting);

        sendbutton = (Button) findViewById(R.id.sendMessage);
        textResponse = (TextView) findViewById(R.id.Response);
        Message = (EditText) findViewById(R.id.message);
        Recipient = (EditText) findViewById(R.id.number);
        disconnect_button = (Button) findViewById(R.id.disconnect_button);
        SenderMac = (EditText) findViewById(R.id.sender_mac);


        sendbutton.setOnClickListener(this);


        // On click listener for the disconnect button
        disconnect_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openMainActivity();
                wifi_disconnect();
            }
        });

        turn= false;
        context = this;
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        }




    public void openMainActivity() {
        android.content.Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }



        // Send Button for the Text Message
        @Override
        public void onClick(View v) {
            switch (v.getId()) {

                case R.id.sendMessage:
                    MessageSend(Message.getText().toString());
                    break;
            }
    }




    //Client Disconnecting Activity
    public void wifi_disconnect () {

        wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);

        if (turn) {

            turnOnOffWifi(context, true);
            turn = true;
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
            turn = false;
            Toast.makeText(getApplicationContext(), "turning off...", Toast.LENGTH_SHORT).show();
        }

    }

    public static void turnOnOffWifi(Context context, boolean isTurnToOn) {
        wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(isTurnToOn);
    }




    // UDP Client Setting Up
    private void MessageSend(final String message) {

        final Handler handler = new Handler();
        Thread thread = new Thread(new Runnable() {

            String stringData;

            @Override
            public void run() {

                // Retrieving Mac Address to a string

                String mac_address = SenderMac.getText().toString();
                String recipient_mac = Recipient.getText().toString();

                String[] sendermac_address = mac_address.split(":");
                String[] recipientmac_address = recipient_mac.split(":");

// convert hex string to byte values
                byte[] sendermacAddressBytes = new byte[6];
                for(int i=0; i<6; i++){
                    Integer hex = Integer.parseInt(sendermac_address[i], 16);
                    sendermacAddressBytes[i] = hex.byteValue();
                }

                byte[] recipientmacAddressBytes = new byte[6];
                for(int j=0; j<6; j++){
                    Integer hex2 = Integer.parseInt(recipientmac_address[j], 16);
                    recipientmacAddressBytes[j] = hex2.byteValue();
                }




                // Socket Client Set Up
                DatagramSocket socket = null;
                try {
                    socket = new DatagramSocket();
                    InetAddress serverAddr = InetAddress.getByName(IpAddress);
                    DatagramPacket packet;

                    // set the first byte
                    byte[] message_array = new byte[1024];
                    message_array[0] = (byte)0x01;

                    // set the second byte for connect/disconnect activity
                    message_array[1] = (byte)0x02;

                    // attach the recipient mac address
                    System.arraycopy(recipientmacAddressBytes,0, message_array, 2, recipientmacAddressBytes.length);

                    // attach the sender mac address
                    System.arraycopy(sendermacAddressBytes, 0, message_array, 8, sendermacAddressBytes.length);

                    // Attaching and sending message
                    System.arraycopy(message.getBytes(), 0, message_array, 14, message.length());

                    packet = new DatagramPacket(message_array, message_array.length, serverAddr, 3333);
                    socket.send(packet);

                    byte[] buf = new byte[4096];
                    packet = new DatagramPacket(buf, buf.length);
                    socket.receive(packet);
                    stringData = new String(buf, 0, packet.getLength());

                }

                catch (IOException e) {
                    e.printStackTrace();
                }
                finally {
                    if (socket!= null){
                        socket.close();
                    }
                }
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        String s = textResponse.getText().toString();
                        if (stringData.trim().length() != 0)
                            textResponse.setText(s + "\n    Server Reply : " + stringData);
                    }
                });

            }
        });

        thread.start();

        }
}

