package com.lso.galleryinsights.Adapter;

import android.content.Context;
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
import com.squareup.picasso.Picasso;

import java.util.List;

public class PieceAdapter extends
        RecyclerView.Adapter<PieceAdapter.ViewHolder> {

    public class ViewHolder extends RecyclerView.ViewHolder {
        public TextView pieceDescriptionTextView;
        public TextView pieceNameYearTextView;
        public ImageView pieceImg;

        public ViewHolder(View itemView) {
            super(itemView);

            pieceDescriptionTextView = (TextView) itemView.findViewById(R.id.pieceDescriptionTextView);
            pieceNameYearTextView = (TextView) itemView.findViewById(R.id.pieceNameYearTextView);
            pieceImg = (ImageView) itemView.findViewById(R.id.pieceImg);
        }
    }

    public List<Piece> mPieces;
    private int currentPosition = 0;
    public PieceAdapter(List<Piece> mPieces) {
        this.mPieces = mPieces;
    }

    @Override
    public PieceAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        LayoutInflater inflater = LayoutInflater.from(context);

        // Inflate the custom layout
        View pieceView = inflater.inflate(R.layout.item_piece, parent, false);

        // Return a new holder instance
        ViewHolder viewHolder = new ViewHolder(pieceView);
        return viewHolder;
    }

    // Involves populating data into the item through holder
    @Override
    public void onBindViewHolder(PieceAdapter.ViewHolder holder, int position) {
        // Get the data model based on position
        Piece piece = mPieces.get(position);

        // Set item views based on your views and data model
        TextView pieceDescriptionTextView = holder.pieceDescriptionTextView;
        TextView pieceNameYearTextView = holder.pieceNameYearTextView;
        ImageView pieceImg = holder.pieceImg;

        pieceNameYearTextView.setText(piece.getName() + ", " + piece.getAuthor() + ", " + piece.getYear());
        pieceDescriptionTextView.setText(piece.getDescriptions().get(0).getDescription());
        Picasso.get().load(piece.getImgLink()).into(pieceImg);
    }

    // Returns the total count of items in the list
    @Override
    public int getItemCount() {
        return mPieces.size();
    }

    public int getCurrentPosition() {
        return currentPosition;
    }

    public void setCurrentPosition(int currentPosition) {
        this.currentPosition = currentPosition;
    }
}
