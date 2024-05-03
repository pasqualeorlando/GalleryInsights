package com.lso.galleryinsights.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.lso.galleryinsights.Presenter.AuthenticationPresenter;
import com.lso.galleryinsights.R;

public class LoginActivity extends AppCompatActivity {

    private EditText emailInput, passwordInput;
    private Button loginButton;
    private TextView clickableTextView;

    private AuthenticationPresenter authenticationPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        authenticationPresenter = new AuthenticationPresenter(this);

        authenticationPresenter.checkUserAlreadyLogged();

        emailInput = (EditText) findViewById(R.id.emailInput);
        passwordInput = (EditText) findViewById(R.id.passwordInput);
        loginButton = (Button) findViewById(R.id.loginButton);
        clickableTextView = (TextView) findViewById(R.id.notRegistered);

        /* for clickable sub-string*/
        String clickableText = getString(R.string.not_registered);
        SpannableString spannableString = new SpannableString(clickableText);
        ClickableSpan clickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View view) {
                Intent i = new Intent(getBaseContext(), RegisterActivity.class);
                startActivity(i);
            }

            @Override
            public void updateDrawState(TextPaint ds) {
                super.updateDrawState(ds);
                ds.setColor(getColor(R.color.blue));
                ds.setUnderlineText(true);
            }
        };
        spannableString.setSpan(clickableSpan, clickableText.indexOf("clicca qui"), clickableText.indexOf("clicca qui") + "clicca qui".length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        clickableTextView.setText(spannableString);
        clickableTextView.setMovementMethod(LinkMovementMethod.getInstance());

        //click listener on login button
        loginButton.setOnClickListener(v -> {
            String email = emailInput.getText().toString();
            String password = passwordInput.getText().toString();

            authenticationPresenter.login(email, password);
        });
    }
}