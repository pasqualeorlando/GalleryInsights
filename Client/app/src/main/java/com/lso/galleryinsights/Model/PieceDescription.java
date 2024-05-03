package com.lso.galleryinsights.Model;

import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;

public class PieceDescription {
    //Fields
    private int pieceId;
    private String description;
    private Expertise expertiseLevel;
    private UserType userType;

    //Constructor
    public PieceDescription(int pieceId, String description, Expertise expertiseLevel, UserType userType) {
        this.pieceId = pieceId;
        this.description = description;
        this.expertiseLevel = expertiseLevel;
        this.userType = userType;
    }

    //Getter & Setter
    public int getPieceId() {
        return pieceId;
    }

    public void setPieceId(int pieceId) {
        this.pieceId = pieceId;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Expertise getExpertiseLevel() {
        return expertiseLevel;
    }

    public void setExpertiseLevel(Expertise expertiseLevel) {
        this.expertiseLevel = expertiseLevel;
    }

    public UserType getUserType() {
        return userType;
    }

    public void setUserType(UserType userType) {
        this.userType = userType;
    }
}
