package com.example.nubapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Bundle;
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

public class TextingActivity extends AppCompatActivity {

    TextView textResponse;
    EditText Message;
    Button sendbutton;
    String IpAddress = "192.168.4.1";//Ip address of esp
    int Port = 3333;//Wifi Server Number to be filled out;
    byte[] buf = new byte[1024];


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_texting);

        sendbutton = (Button) findViewById(R.id.sendMessage);
        textResponse = (TextView) findViewById(R.id.Response);
        Message = (EditText) findViewById(R.id.message);

        sendbutton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                MyClientTask myClientTask = new MyClientTask(Message.getText().toString());
                myClientTask.execute();
            }

        });

    }
    @SuppressLint("StaticFieldLeak")
    public class MyClientTask extends AsyncTask<Void,Void,Void> {
        String Response = "";
        String msgToServer;

        MyClientTask(String msgTo) {
            msgToServer = msgTo;
        }

        @Override
        protected Void doInBackground(Void... voids) {
            InetAddress ip_addr;
            DatagramPacket packet;
            DatagramSocket socket;

            try {
                ip_addr = InetAddress.getByName(IpAddress);
                socket = new DatagramSocket();
                packet = new DatagramPacket(buf, buf.length, ip_addr, Port);

                socket.send(packet);
                socket.close();
            }
            catch (UnknownHostException e) {
                e.printStackTrace();
            }
            catch (SocketException e) {
                e.printStackTrace();
            }
            catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

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
