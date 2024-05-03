package com.lso.galleryinsights.Model.enums;

public enum Expertise {
    AMATEUR("Principiante"),
    STANDARD("Standard"),
    EXPERT("Esperto");

    private final String name;

    Expertise(String s) {
        name = s;
    }

    public static Expertise fromString(String text) {
        for (Expertise exp : Expertise.values()) {
            if (exp.name.equalsIgnoreCase(text)) {
                return exp;
            }
        }
        return null;
    }

    public static Expertise fromEnumString(String text) {
        for (Expertise exp : Expertise.values()) {
            if (exp.name().equalsIgnoreCase(text)) {
                return exp;
            }
        }
        return null;
    }
}
