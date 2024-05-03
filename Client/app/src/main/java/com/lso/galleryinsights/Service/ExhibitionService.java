package com.lso.galleryinsights.Service;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Service.Interfaces.IExhibitionService;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class ExhibitionService implements IExhibitionService {
    public ArrayList<Exhibition> getAllExhibitions() {
        ArrayList<Exhibition> toReturn = new ArrayList<Exhibition>();

        String operation = "get_exhibitions";

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

            while(reader.ready()) {
                responseBuffer = reader.readLine();
                responseBuffer = responseBuffer.replace("\u0000", "");

                if(responseBuffer.equals("START"))
                    continue;
                if(responseBuffer.isEmpty() || responseBuffer.equals("END"))
                    break;

                String[] fields = responseBuffer.split(":");

                int id = Integer.parseInt(fields[0]);
                String name = fields[1];
                String description = fields[2];

                toReturn.add(new Exhibition(id, name, description));
            }
        } catch(Exception e) {
            return null;
        }

        return toReturn;
    }
}
