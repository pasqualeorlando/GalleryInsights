package com.lso.galleryinsights.Adapter;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.lso.galleryinsights.Model.Exhibition;
import com.lso.galleryinsights.Model.Piece;
import com.lso.galleryinsights.R;
import com.lso.galleryinsights.View.PiecesActivity;

import java.util.List;

public class ExhibitionAdapter extends
        RecyclerView.Adapter<ExhibitionAdapter.ViewHolder> {

    public class ViewHolder extends RecyclerView.ViewHolder {
        public TextView exhibitionNameTextView;
        public TextView exhibitionDescriptionTextView;
        public Button detailsButton;

        public ViewHolder(View itemView) {
            super(itemView);

            exhibitionNameTextView = (TextView) itemView.findViewById(R.id.exhibitionNameTextView);
            exhibitionDescriptionTextView = (TextView) itemView.findViewById(R.id.exhibitionDescriptionTextView);
            detailsButton = (Button) itemView.findViewById(R.id.detailsButton);
        }
    }

    public List<Exhibition> mExhibitions;

    public ExhibitionAdapter(List<Exhibition> mExhibitions) {
        this.mExhibitions = mExhibitions;
    }

    @Override
    public ExhibitionAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        LayoutInflater inflater = LayoutInflater.from(context);

        // Inflate the custom layout
        View exhibitionView = inflater.inflate(R.layout.item_exhibition, parent, false);

        // Return a new holder instance
        ViewHolder viewHolder = new ViewHolder(exhibitionView);
        return viewHolder;
    }

    // Involves populating data into the item through holder
    @Override
    public void onBindViewHolder(ExhibitionAdapter.ViewHolder holder, int position) {
        // Get the data model based on position
        Exhibition exhibition = mExhibitions.get(position);

        // Set item views based on your views and data model
        TextView exhibitionNameTextView = holder.exhibitionNameTextView;
        TextView exhibitionDescriptionTextView = holder.exhibitionDescriptionTextView;
        Button detailsButton = holder.detailsButton;

        exhibitionNameTextView.setText(exhibition.getName());
        exhibitionDescriptionTextView.setText(exhibition.getDescription());
        detailsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(view.getContext(), PiecesActivity.class);
                i.putExtra("exhibition", exhibition);
                view.getContext().startActivity(i);
            }
        });
    }

    // Returns the total count of items in the list
    @Override
    public int getItemCount() {
        return mExhibitions.size();
    }
}
