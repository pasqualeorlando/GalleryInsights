package com.lso.galleryinsights.Presenter;

import android.app.Activity;
import android.content.Intent;
import android.text.TextUtils;
import android.util.Log;
import android.util.Patterns;

import com.lso.galleryinsights.Model.User;
import com.lso.galleryinsights.Service.AuthenticationService;
import com.lso.galleryinsights.Utility;
import com.lso.galleryinsights.View.LoginActivity;
import com.lso.galleryinsights.View.RegisterActivity;
import com.lso.galleryinsights.View.UserTypeSelectionActivity;

import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

import es.dmoral.toasty.Toasty;

public class AuthenticationPresenter {
    private final Activity activity;

    private final AuthenticationService authenticationService;

    public AuthenticationPresenter(Activity activity) {
        this.activity = activity;
        authenticationService = new AuthenticationService();
    }

    public void register(String name, String email, String password, String confirmPassword) {
        if(TextUtils.isEmpty(email) || !Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
            activity.runOnUiThread(() -> Toasty.error(activity,"Email non conforme", Toasty.LENGTH_SHORT,true).show());
        }else if(password.trim().isEmpty()) {
            activity.runOnUiThread(() -> Toasty.error(activity, "Inserire password (non valgono solo spazi bianchi)", Toasty.LENGTH_SHORT, true).show());
        } else if(name.trim().isEmpty()) {
            activity.runOnUiThread(() -> Toasty.error(activity, "Inserire un nome (non valgono solo spazi bianchi)", Toasty.LENGTH_SHORT, true).show());
        } else if(!password.equals(confirmPassword)) {
            activity.runOnUiThread(() -> Toasty.error(activity, "Le due password devono essere uguali", Toasty.LENGTH_SHORT, true).show());
        } else if(!password.matches("(.*){8,255}")) {
            activity.runOnUiThread(() -> Toasty.error(activity, "La password deve essere lunga almeno 8 caratteri e al massimo 255", Toasty.LENGTH_SHORT, true).show());
        } else {
            //verification done, insert user into database
            try {
                CompletableFuture<Integer> future = CompletableFuture.supplyAsync(new Supplier<Integer>() {
                    @Override
                    public Integer get() {
                        return authenticationService.register(name, email, password);
                    }
                });
                int registered = future.get();

                if(registered == 0) {
                    activity.runOnUiThread(() -> Toasty.success(activity, "Utente registrato con successo", Toasty.LENGTH_SHORT, true).show());
                    Intent openLogin = new Intent(activity.getBaseContext(), LoginActivity.class);
                    activity.runOnUiThread(() -> activity.startActivity(openLogin));
                } else if(registered == 1)
                    activity.runOnUiThread(() -> Toasty.error(activity, "Email già registrata", Toasty.LENGTH_SHORT, true).show());
                else
                    activity.runOnUiThread(() -> Toasty.error(activity, "Errore durante la registrazione. Riprova più tardi", Toasty.LENGTH_SHORT, true).show());
            } catch(Exception e) {
                activity.runOnUiThread(() -> Toasty.error(activity, "Errore durante la connessione al server. Riprova più tardi", Toasty.LENGTH_SHORT, true).show());
            }

        }
    }

    public void login(String email, String password) {
        try {
            CompletableFuture<User> future = CompletableFuture.supplyAsync(new Supplier<User>() {
                @Override
                public User get() {
                    return authenticationService.login(email, password);
                }
            });

            if(TextUtils.isEmpty(email) || !Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
                activity.runOnUiThread(() -> Toasty.error(activity,"Email non conforme", Toasty.LENGTH_SHORT,true).show());
            }else if(password.trim().isEmpty()) {
                activity.runOnUiThread(() -> Toasty.error(activity,"Inserire password (non valgono solo spazi bianchi)", Toasty.LENGTH_SHORT,true).show());
            }else{
                User logged = future.get();
                if(logged != null) {
                    activity.runOnUiThread(() -> Toasty.success(activity, "Login effettuato", Toasty.LENGTH_SHORT, true).show());

                    //save user in shared pref
                    Utility.saveLoggedUserId(activity, logged);

                    Intent openUserTypeSelectionActivity = new Intent(activity.getBaseContext(), UserTypeSelectionActivity.class);
                    activity.runOnUiThread(() -> activity.startActivity(openUserTypeSelectionActivity));
                }
                else
                    activity.runOnUiThread(() -> Toasty.error(activity,"Credenziali errate", Toasty.LENGTH_SHORT,true).show());
            }
        } catch(Exception e) {
            activity.runOnUiThread(() -> Toasty.error(activity, "Errore durante la connessione al server. Riprova più tardi", Toasty.LENGTH_SHORT, true).show());
        }
    }

    public void logout() {
        Utility.clearSavedPreferences(activity);

        Intent i = new Intent(activity.getBaseContext(), LoginActivity.class);
        activity.runOnUiThread(() -> activity.startActivity(i));
    }

    public void checkUserAlreadyLogged() {
        if(Utility.getLoggedUserId(activity) != -1) {
            Intent i = new Intent(activity.getBaseContext(), UserTypeSelectionActivity.class);
            activity.runOnUiThread(() -> activity.startActivity(i));
        }
    }
}
