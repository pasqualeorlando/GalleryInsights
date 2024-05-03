package com.lso.galleryinsights.Model;

import java.util.List;

public class Piece {
    //Fields
    private int pieceId;
    private String name;
    private String author;
    private String year;
    private String imgLink;
    private List<PieceDescription> descriptions;

    //Constructor
    public Piece(int pieceId, String name, String author, String year, String imgLink, List<PieceDescription> descriptions) {
        this.pieceId = pieceId;
        this.name = name;
        this.author = author;
        this.year = year;
        this.imgLink = imgLink;
        this.descriptions = descriptions;
    }

    //Getter & Setter
    public int getPieceId() {
        return pieceId;
    }

    public void setPieceId(int pieceId) {
        this.pieceId = pieceId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getYear() {
        return year;
    }

    public void setYear(String year) {
        this.year = year;
    }

    public String getImgLink() {
        return imgLink;
    }

    public void setImgLink(String imgLink) {
        this.imgLink = imgLink;
    }

    public List<PieceDescription> getDescriptions() {
        return descriptions;
    }

    public void setDescriptions(List<PieceDescription> descriptions) {
        this.descriptions = descriptions;
    }
}
