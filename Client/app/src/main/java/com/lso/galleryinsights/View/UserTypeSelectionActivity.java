package com.lso.galleryinsights.View;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.ColorStateList;
import android.os.Bundle;
import android.util.TypedValue;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import com.lso.galleryinsights.Model.enums.Expertise;
import com.lso.galleryinsights.Model.enums.UserType;
import com.lso.galleryinsights.Presenter.UserTypeSelectionPresenter;
import com.lso.galleryinsights.R;
public class UserTypeSelectionActivity extends AppCompatActivity {

    private RadioGroup userTypeRadioGroup, userExpertiseRadioGroup;
    private Button goAheadButton;

    private UserTypeSelectionPresenter userTypeSelectionPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_type_selection);

        userTypeSelectionPresenter = new UserTypeSelectionPresenter(this);

        userTypeRadioGroup = (RadioGroup) findViewById(R.id.userTypeRadioGroup);
        RadioGroup.LayoutParams params = new RadioGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        params.setMargins(15, 15, 15, 15);

        UserType savedUserType = userTypeSelectionPresenter.getUserTypePreference();
        Expertise savedExpertise = userTypeSelectionPresenter.getExpertisePreference();

        ColorStateList colorStateList = new ColorStateList(
            new int[][]{
                    new int[]{-android.R.attr.state_enabled}, //disabled
                    new int[]{android.R.attr.state_enabled} //enabled
            },
            new int[] {
                    getColor(R.color.black), //disabled
                    getColor(R.color.blue) //enabled
            }
        );

        String[] userTypeStrings = getResources().getStringArray(R.array.user_type_strings);
        for (int i = 0; i < userTypeStrings.length; i++) {
            RadioButton rb = new RadioButton(this);
            rb.setText(userTypeStrings[i]);
            rb.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 20);
            rb.setLayoutParams(params);

            rb.setButtonTintList(colorStateList);

            userTypeRadioGroup.addView(rb);
            if(i == savedUserType.ordinal())
                rb.setChecked(true);
        }

        userExpertiseRadioGroup = (RadioGroup) findViewById(R.id.userExpertiseRadioGroup);
        String[] userExpertiseStrings = getResources().getStringArray(R.array.user_expertise_strings);
        for (int i = 0; i < userExpertiseStrings.length; i++) {
            RadioButton rb = new RadioButton(this);
            rb.setText(userExpertiseStrings[i]);
            rb.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 20);
            rb.setLayoutParams(params);

            rb.setButtonTintList(colorStateList);

            userExpertiseRadioGroup.addView(rb);
            if(i == savedExpertise.ordinal())
                rb.setChecked(true);
        }

        goAheadButton = (Button) findViewById(R.id.goAheadButton);
        goAheadButton.setOnClickListener(v -> {
            int selectedUserTypeId = userTypeRadioGroup.getCheckedRadioButtonId();
            int selectedExpertiseId = userExpertiseRadioGroup.getCheckedRadioButtonId();

            String userType = ((RadioButton) findViewById(selectedUserTypeId)).getText().toString();
            String userExpertise = ((RadioButton) findViewById(selectedExpertiseId)).getText().toString();

            userTypeSelectionPresenter.savePreferences(userType, userExpertise);
        });
    }
}