package in.tecresearch.ievm.environment;

import io.github.cdimascio.dotenv.Dotenv;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;

public class Environment {
    public static void loadEnv() {
        try {
            Dotenv dotenv = Dotenv.configure()
                    .filename(".env")
                    .ignoreIfMissing()
                    .load();

            String activeProfile = System.getProperty("env",
                    dotenv.get("SPRING_PROFILES_ACTIVE", "dev"));

            // Get raw URI based on profile
            String mongoUri = "prod".equalsIgnoreCase(activeProfile)
                    ? dotenv.get("MONGODB_URI_PROD")
                    : dotenv.get("MONGODB_URI_DEV");

            // Set system properties
            System.setProperty("MONGODB_URI", mongoUri);
            System.setProperty("DATABASE_NAME", dotenv.get("DATABASE_NAME"));
            System.setProperty("ACTIVE_PROFILE", activeProfile.toLowerCase());

            System.out.println("Active Profile: " + activeProfile);
            System.out.println("Using MongoDB URI: " + mongoUri);

        } catch (Exception e) {
            System.err.println("Failed to load environment: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }

}