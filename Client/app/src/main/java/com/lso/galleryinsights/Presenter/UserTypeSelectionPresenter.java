package com.lso.galleryinsights.Presenter;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;
import com.lso.galleryinsights.Utility;
import com.lso.galleryinsights.View.ExhibitionsActivity;
import com.lso.galleryinsights.View.UserTypeSelectionActivity;

import com.lso.galleryinsights.R;

import java.util.ArrayList;
import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

import es.dmoral.toasty.Toasty;

public class UserTypeSelectionPresenter {
    private final UserTypeSelectionActivity userTypeSelectionActivity;

    public UserTypeSelectionPresenter(UserTypeSelectionActivity userTypeSelectionActivity) {
        this.userTypeSelectionActivity = userTypeSelectionActivity;
    }

    public void savePreferences(String userTypeString, String expertiseString) {
        Utility.saveUserPreferences(userTypeSelectionActivity, userTypeString, expertiseString);
    }

    public UserType getUserTypePreference() {
        return Utility.getUserTypePreference(userTypeSelectionActivity);
    }

    public Expertise getExpertisePreference() {
        return Utility.getExpertisePreference(userTypeSelectionActivity);
    }
}
