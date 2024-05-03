package com.lso.galleryinsights.Presenter;

import android.util.Log;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Model.Piece;
import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;
import com.lso.galleryinsights.Service.PieceService;
import com.lso.galleryinsights.Utility;
import com.lso.galleryinsights.View.PiecesActivity;

import java.util.ArrayList;
import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

public class PiecesPresenter {
    private final PiecesActivity piecesActivity;
    private final PieceService pieceService;

    public PiecesPresenter(PiecesActivity piecesActivity) {
        this.piecesActivity = piecesActivity;
        pieceService = new PieceService();
    }

    public ArrayList<Piece> getPieces(Exhibition exhibition) {

        UserType userType = Utility.getUserTypePreference(piecesActivity);
        Expertise expertise = Utility.getExpertisePreference(piecesActivity);

        try {
            CompletableFuture<ArrayList<Piece>> future = CompletableFuture.supplyAsync(new Supplier<ArrayList<Piece>>() {
                @Override
                public ArrayList<Piece> get() {
                    return pieceService.getPieces(exhibition, userType, expertise);
                }
            });

            return future.get();
        } catch(Exception e) {
            return null;
        }
    }
}
