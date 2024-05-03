package com.lso.galleryinsights.Service.Interfaces;

import com.lso.galleryinsights.Model.User;

public interface IAuthenticationService {

    public User login(String email, String password);
}
