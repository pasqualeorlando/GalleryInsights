package com.lso.galleryinsights.Service;

import android.util.Log;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Model.Piece;
import com.lso.galleryinsights.Model.PieceDescription;
import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;
import com.lso.galleryinsights.Service.Interfaces.IPieceService;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class PieceService implements IPieceService {

    public ArrayList<Piece> getPieces(Exhibition exhibition, UserType userType, Expertise expertise) {
        ArrayList<Piece> toReturn = new ArrayList<Piece>();

        String operation = "get_pieces " + exhibition.getExhibtionId() + " " + userType.name() + ":" + expertise.name();

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
                String author = fields[2];
                String year = fields[3];
                String imgLink = fields[4] + ":" + fields[5];
                String description = fields[6];

                PieceDescription pieceDescription = new PieceDescription(id, description, expertise, userType);
                List<PieceDescription> pieceDescriptionList = new ArrayList<PieceDescription>();
                pieceDescriptionList.add(pieceDescription);

                Piece piece = new Piece(id, name, author, year, imgLink, pieceDescriptionList);

                toReturn.add(piece);
            }

            exhibition.setPieceList(toReturn);
            SocketUtil.closeConnection(socket);
            return toReturn;
        } catch(Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
