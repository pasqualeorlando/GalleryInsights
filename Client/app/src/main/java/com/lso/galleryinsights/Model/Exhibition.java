package com.lso.galleryinsights.Model;

import java.io.Serializable;
import java.util.List;

public class Exhibition implements Serializable {
    //Fields
    private int exhibtionId;
    private String name;
    private String description;
    private List<Piece> pieceList;

    //Constructors
    public Exhibition(int exhibtionId, String name, String description) {
        this.exhibtionId = exhibtionId;
        this.name = name;
        this.description = description;
    }

    public Exhibition(int exhibtionId, String name, String description, List<Piece> pieceList) {
        this.exhibtionId = exhibtionId;
        this.name = name;
        this.description = description;
        this.pieceList = pieceList;
    }

    //Getter & Setter
    public int getExhibtionId() {
        return exhibtionId;
    }

    public void setExhibtionId(int exhibtionId) {
        this.exhibtionId = exhibtionId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public List<Piece> getPieceList() {
        return pieceList;
    }

    public void setPieceList(List<Piece> pieceList) {
        this.pieceList = pieceList;
    }
}
