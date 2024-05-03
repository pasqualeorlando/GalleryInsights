package com.lso.galleryinsights.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.viewpager2.widget.ViewPager2;

import android.content.Intent;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.material.carousel.CarouselLayoutManager;
import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.lso.galleryinsights.Adapter.PieceAdapter;
import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Model.Piece;
import com.lso.galleryinsights.Presenter.AuthenticationPresenter;
import com.lso.galleryinsights.Presenter.PiecesPresenter;
import com.lso.galleryinsights.R;

import java.util.ArrayList;

public class PiecesActivity extends AppCompatActivity {

    private Button logout, exhibitions;
    private FloatingActionButton backButton, nextButton;
    private TextView exhibitionName, exhibitionDescription;
    private PiecesPresenter piecesPresenter;
    private ViewPager2 piecesViewPager;
    private AuthenticationPresenter authenticationPresenter;
    private ArrayList<Piece> pieces;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pieces);

        Exhibition passedExhibition = null;
        Bundle extras = getIntent().getExtras();
        if(extras != null)
            passedExhibition = (Exhibition) extras.getSerializable("exhibition");

        piecesPresenter = new PiecesPresenter(this);
        authenticationPresenter = new AuthenticationPresenter(this);

        logout = (Button) findViewById(R.id.logoutButton);
        exhibitions = (Button) findViewById(R.id.exhibitionsButton);
        piecesViewPager = (ViewPager2) findViewById(R.id.piecesViewPager);
        backButton = (FloatingActionButton) findViewById(R.id.backButton);
        nextButton = (FloatingActionButton) findViewById(R.id.nextButton);
        exhibitionName = (TextView) findViewById(R.id.exhibitionTextView);
        exhibitionDescription = (TextView) findViewById(R.id.descriptionTextView);

        exhibitionName.setText(passedExhibition.getName());
        exhibitionDescription.setText(passedExhibition.getDescription());

        pieces = piecesPresenter.getPieces(passedExhibition);

        PieceAdapter pieceAdapter = new PieceAdapter(pieces);
        piecesViewPager.setAdapter(pieceAdapter);

        piecesViewPager.registerOnPageChangeCallback(new ViewPager2.OnPageChangeCallback() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
                super.onPageScrolled(position, positionOffset, positionOffsetPixels);
            }

            @Override
            public void onPageSelected(int position) {
                super.onPageSelected(position);
            }

            @Override
            public void onPageScrollStateChanged(int state) {
                super.onPageScrollStateChanged(state);
            }
        });

        piecesViewPager.addItemDecoration(new RecyclerView.ItemDecoration() {
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

        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                authenticationPresenter.logout();
            }
        });

        exhibitions.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(PiecesActivity.this, ExhibitionsActivity.class);
                startActivity(i);
            }
        });

        backButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(pieceAdapter.getCurrentPosition() > 0) {
                    pieceAdapter.setCurrentPosition(pieceAdapter.getCurrentPosition() - 1);
                    piecesViewPager.setCurrentItem(pieceAdapter.getCurrentPosition(), true);
                }
            }
        });

        nextButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(pieceAdapter.getCurrentPosition() < (pieceAdapter.getItemCount() - 1)) {
                    pieceAdapter.setCurrentPosition(pieceAdapter.getCurrentPosition() + 1);
                    piecesViewPager.setCurrentItem(pieceAdapter.getCurrentPosition(), true);
                }
            }
        });
    }
}