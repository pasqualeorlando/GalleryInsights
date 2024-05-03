package com.lso.galleryinsights.Model.enums;

public enum UserType {
    SINGLE("Singolo"),
    GENERIC_GROUP("Gruppo generico"),
    SCHOOL_GROUP("Scolaresca"),
    FAMILY("Famiglia");

    private final String name;

    UserType(String s) {
        name = s;
    }

    public static UserType fromString(String text) {
        for (UserType userType : UserType.values()) {
            if (userType.name.equalsIgnoreCase(text)) {
                return userType;
            }
        }
        return null;
    }

    public static UserType fromEnumString(String text) {
        for (UserType userType : UserType.values()) {
            if (userType.name().equalsIgnoreCase(text)) {
                return userType;
            }
        }
        return null;
    }
}
