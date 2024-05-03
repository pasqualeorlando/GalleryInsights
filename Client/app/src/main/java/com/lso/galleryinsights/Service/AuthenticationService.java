package com.lso.galleryinsights.Service;

import com.lso.galleryinsights.Model.User;
import com.lso.galleryinsights.Service.Interfaces.IAuthenticationService;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;

public class AuthenticationService implements IAuthenticationService {
    public AuthenticationService() {}

    public User login(String email, String password) {
        User logged = null;
        String operation = "login " + email + ":" + password;

        try {
            //open socket connection
            Socket socket = SocketUtil.openConnection();

            //request output stream
            socket.getOutputStream().write(operation.getBytes());

            //buffered reader
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            Thread.sleep(1000);

            //get response
            String responseBuffer;
            if(reader.ready()) {
                responseBuffer = reader.readLine();
                responseBuffer = responseBuffer.replace("\u0000", "");

                if(!responseBuffer.equals("bad credentials")) {
                    String[] fields = responseBuffer.split(":");
                    int id = Integer.parseInt(fields[0]);
                    String name = fields[1];
                    logged = new User(id, name, email, password);
                }
            }

            SocketUtil.closeConnection(socket);
            return logged;
        } catch(Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public int register(String name, String email, String password) {
        int registered = 2;
        String operation = "register " + name + ":" + email + ":" + password;

        try {
            //open socket connection
            Socket socket = SocketUtil.openConnection();

            //request output stream
            socket.getOutputStream().write(operation.getBytes());

            //buffered reader
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            Thread.sleep(1000);

            //get response
            String responseBuffer;
            if(reader.ready()) {
                responseBuffer = reader.readLine();
                responseBuffer = responseBuffer.replace("\u0000", "");

                //Log.e("RESPONSE BUFFER", responseBuffer);
                if(responseBuffer.equals("user registered"))
                    registered = 0;
                else if(responseBuffer.equals("email already registered"))
                    registered = 1;
            }

            SocketUtil.closeConnection(socket);
            return registered;
        } catch(Exception e) {
            e.printStackTrace();
            return registered;
        }
    }
}
