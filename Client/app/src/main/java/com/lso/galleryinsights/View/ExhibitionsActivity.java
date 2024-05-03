package com.lso.galleryinsights.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.graphics.Rect;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.lso.galleryinsights.Adapter.ExhibitionAdapter;
import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Presenter.AuthenticationPresenter;
import com.lso.galleryinsights.Presenter.ExhibitionsPresenter;
import com.lso.galleryinsights.R;

import java.util.ArrayList;

public class ExhibitionsActivity extends AppCompatActivity {

    private Button logout, editUser;
    private RecyclerView exhibitionsRecyclerView;

    private ExhibitionsPresenter exhibitionsPresenter;
    private AuthenticationPresenter authenticationPresenter;

    private ArrayList<Exhibition> exhibitions;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_exhibitions);

        exhibitionsPresenter = new ExhibitionsPresenter(this);
        authenticationPresenter = new AuthenticationPresenter(this);

        logout = (Button) findViewById(R.id.logoutButton);
        editUser = (Button) findViewById(R.id.editUserButton);
        exhibitionsRecyclerView = (RecyclerView) findViewById(R.id.exhibitionsRecyclerView);

        exhibitions = exhibitionsPresenter.getExhibitions();

        ExhibitionAdapter exhibitionAdapter = new ExhibitionAdapter(exhibitions);

        exhibitionsRecyclerView.setAdapter(exhibitionAdapter);

        exhibitionsRecyclerView.setLayoutManager(new GridLayoutManager(this, 3));

        exhibitionsRecyclerView.addItemDecoration(new RecyclerView.ItemDecoration() {
            @Override
            public void getItemOffsets(@NonNull Rect outRect, @NonNull View view, @NonNull RecyclerView parent, @NonNull RecyclerView.State state) {
                int position = parent.getChildAdapterPosition(view); // item position
                int spanCount = 3;
                int spacing = 40;//spacing between views in grid

                if (position >= 0) {
                    int column = position % spanCount; // item column

                    outRect.left = spacing - column * spacing / spanCount; // spacing - column * ((1f / spanCount) * spacing)
                    outRect.right = (column + 1) * spacing / spanCount; // (column + 1) * ((1f / spanCount) * spacing)

                    if (position < spanCount) { // top edge
                        outRect.top = spacing;
                    }
                    outRect.bottom = spacing; // item bottom
                } else {
                    outRect.left = 0;
                    outRect.right = 0;
                    outRect.top = 0;
                    outRect.bottom = 0;
                }
            }
        });

        editUser.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(ExhibitionsActivity.this, UserTypeSelectionActivity.class);
                startActivity(i);
            }
        });

        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                authenticationPresenter.logout();
            }
        });
    }
}