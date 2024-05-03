package com.lso.galleryinsights.Service;

import java.io.IOException;
import java.net.Socket;

public class SocketUtil {
    private static final String ADDRESS = "10.0.2.2";
    private static final int PORT = 8888;

    //Constructor
    private SocketUtil() {}

    public static Socket openConnection() throws IOException {
        Socket socket = new Socket(ADDRESS, PORT);
        socket.setSoTimeout(5 * 1000);
        return socket;
    }

    public static void closeConnection(Socket socket) throws IOException {
        socket.close();
    }
}
