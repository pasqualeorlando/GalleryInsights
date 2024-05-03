package com.lso.galleryinsights;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import com.lso.galleryinsights.Model.User;
import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;
import com.lso.galleryinsights.View.ExhibitionsActivity;
import com.lso.galleryinsights.R;

import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

import es.dmoral.toasty.Toasty;

public class Utility {

    public static void saveUserPreferences(Activity activity, String userTypeString, String expertiseString) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                UserType userType = UserType.fromString(userTypeString);
                Expertise expertise = Expertise.fromString(expertiseString);

                SharedPreferences sharedPref = activity.getSharedPreferences(activity.getString(R.string.preference_file_key), Context.MODE_PRIVATE);
                SharedPreferences.Editor editor = sharedPref.edit();
                editor.putString(activity.getString(R.string.shared_pref_user_type), userType.name());
                editor.putString(activity.getString(R.string.shared_pref_expertise), expertise.name());
                editor.apply();

                activity.runOnUiThread(() -> Toasty.success(activity, "Preferenze salvate", Toasty.LENGTH_SHORT, true).show());

                Intent openExhibitionsActivity = new Intent(activity, ExhibitionsActivity.class);
                activity.runOnUiThread(() -> activity.startActivity(openExhibitionsActivity));
            }
        });

        thread.start();
    }

    public static void saveLoggedUserId(Activity activity, User user) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                SharedPreferences sharedPref = activity.getSharedPreferences(activity.getString(R.string.preference_file_key), Context.MODE_PRIVATE);
                SharedPreferences.Editor editor = sharedPref.edit();

                editor.putInt(activity.getString(R.string.shared_pref_userid), user.getUserId());
                editor.apply();

                //activity.runOnUiThread(() -> Toasty.success(activity, "Preferenze salvate", Toasty.LENGTH_SHORT, true).show());
            }
        });

        thread.start();
    }

    public static UserType getUserTypePreference(Activity activity) {
        CompletableFuture<UserType> future = CompletableFuture.supplyAsync(new Supplier<UserType>() {
            @Override
            public UserType get() {
                SharedPreferences sharedPref = activity.getSharedPreferences(activity.getString(R.string.preference_file_key), Context.MODE_PRIVATE);

                String userTypeString = sharedPref.getString(activity.getString(R.string.shared_pref_user_type), null);

                if(userTypeString == null)
                    return UserType.SINGLE;
                else return UserType.fromEnumString(userTypeString);
            }
        });

        try {
            return future.get();
        } catch(Exception e) {
            return UserType.SINGLE;
        }
    }

    public static Expertise getExpertisePreference(Activity activity) {
        CompletableFuture<Expertise> future = CompletableFuture.supplyAsync(new Supplier<Expertise>() {
            @Override
            public Expertise get() {
                SharedPreferences sharedPref = activity.getSharedPreferences(activity.getString(R.string.preference_file_key), Context.MODE_PRIVATE);

                String expertiseString = sharedPref.getString(activity.getString(R.string.shared_pref_expertise), null);

                if(expertiseString == null)
                    return Expertise.STANDARD;
                else return Expertise.fromEnumString(expertiseString);
            }
        });

        try {
            return future.get();
        } catch(Exception e) {
            return Expertise.STANDARD;
        }
    }

    public static int getLoggedUserId(Activity activity) {
        CompletableFuture<Integer> future = CompletableFuture.supplyAsync(new Supplier<Integer>() {
            @Override
            public Integer get() {
                SharedPreferences sharedPref = activity.getSharedPreferences(activity.getString(R.string.preference_file_key), Context.MODE_PRIVATE);

                Integer loggedUserId = sharedPref.getInt(activity.getString(R.string.shared_pref_userid), Integer.valueOf(-1));

                return loggedUserId.intValue();
            }
        });

        try {
            return future.get();
        } catch(Exception e) {
            return -1;
        }
    }

    public static void clearSavedPreferences(Activity activity) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                SharedPreferences sharedPref = activity.getSharedPreferences(activity.getString(R.string.preference_file_key), Context.MODE_PRIVATE);

                SharedPreferences.Editor editor = sharedPref.edit();
                editor.clear();
                editor.commit();
            }
        });

        thread.start();
    }
}
