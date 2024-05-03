package com.lso.galleryinsights.Service.Interfaces;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Model.Piece;
import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;

import java.util.List;

public interface IPieceService {
    public List<Piece> getPieces(Exhibition exhibition, UserType userType, Expertise expertise);
}
