package com.lso.galleryinsights.Presenter;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Service.ExhibitionService;
import com.lso.galleryinsights.View.ExhibitionsActivity;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

import es.dmoral.toasty.Toasty;

public class ExhibitionsPresenter {

    private final ExhibitionsActivity exhibitionsActivity;
    private final ExhibitionService exhibitionService;

    public ExhibitionsPresenter(ExhibitionsActivity exhibitionsActivity) {
        this.exhibitionsActivity = exhibitionsActivity;
        exhibitionService = new ExhibitionService();
    }

    public ArrayList<Exhibition> getExhibitions() {
        try {
            CompletableFuture<ArrayList<Exhibition>> future = CompletableFuture.supplyAsync(new Supplier<ArrayList<Exhibition>>() {
                @Override
                public ArrayList<Exhibition> get() {
                    return exhibitionService.getAllExhibitions();
                }
            });

            return future.get();
        } catch(Exception e) {
            return null;
        }
    }
}
