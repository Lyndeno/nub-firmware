package com.example.nubapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class TextingActivity extends AppCompatActivity implements View.OnClickListener {

    TextView textResponse;
    EditText Message;
    Button sendbutton;
    String IpAddress = "192.168.4.1";//Ip address of esp
    int Port = 3333;//Wifi Server Number to be filled out;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_texting);

        sendbutton = (Button) findViewById(R.id.sendMessage);
        textResponse = (TextView) findViewById(R.id.Response);
        Message = (EditText) findViewById(R.id.message);

        sendbutton.setOnClickListener(this);
        }

        @Override
        public void onClick(View v) {
            switch (v.getId()) {

                case R.id.sendMessage:
                    MessageSend(Message.getText().toString());
                    break;
            }
    }

    private void MessageSend(final String message) {

        final Handler handler = new Handler();
        Thread thread = new Thread(new Runnable() {

            String stringData;

            @Override
            public void run() {

                DatagramSocket socket = null;
                try {
                    socket = new DatagramSocket();
                    InetAddress serverAddr = InetAddress.getByName(IpAddress);
                    DatagramPacket packet;
                    packet = new DatagramPacket(message.getBytes(), message.length(), serverAddr, 3333);
                    socket.send(packet);

                    byte[] buf = new byte[1024];
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
                            textResponse.setText(s + "\nFrom Server : " + stringData);

                    }
                });
            }
        });

        thread.start();

        }
}

       /* @Override
        protected Void doInBackground(Void... arg0) {
            Socket socket = null;
            DataOutputStream dataOutputStream = null;
            DataInputStream dataInputStream = null;

            try {
                socket = new Socket(IpAddress, Port);
                dataOutputStream = new DataOutputStream(socket.getOutputStream());
                dataInputStream = new DataInputStream(socket.getInputStream());

                if(!msgToServer.equals(""))
                    dataOutputStream.writeUTF(msgToServer + "$");

                Response = dataInputStream.readUTF();

            }
            catch (IOException e) { }
            finally {
                if (socket != null) {
                    try {
                        socket.close();
                    }
                    catch (IOException e) {}
                }
                if (dataOutputStream != null) {
                    try {
                        dataOutputStream.close();
                    }
                    catch (IOException e) {}
                }
                if (dataInputStream != null) {
                    try {
                        dataInputStream.close();
                    }
                    catch (IOException e) {}
                }
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) {
            textResponse.setText(Response);
            super.onPostExecute(result);
        }
    }*/
