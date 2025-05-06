package in.tecresearch.ievm.environment;

import io.github.cdimascio.dotenv.Dotenv;

public class Environment {
    public static void loadEnv(){

        // Load corresponding .env file
        Dotenv dotenv = Dotenv.configure()
                .filename(".env")
                .load();

        // Get the environment profile passed via command-line argument or default to 'dev'
        String env = System.getProperty("env", dotenv.get("SPRING_PROFILES_ACTIVE"));

        if("production".equalsIgnoreCase(env)){
            System.setProperty("MONGODB_URI",dotenv.get("MONGODB_URI_PROD"));
            System.setProperty("spring.profiles.active", "production");
        }else{
            System.setProperty("MONGODB_URI",dotenv.get("MONGODB_URI_DEV"));
            System.setProperty("spring.profiles.active", "dev");
        }
        System.out.println("Deployment Profile: "+env);
    }
}
