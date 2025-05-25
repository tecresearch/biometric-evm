package in.tecresearch.ievm;


import com.mongodb.client.MongoClient;
import in.tecresearch.ievm.environment.Environment;
import io.swagger.v3.oas.annotations.OpenAPIDefinition;
import io.swagger.v3.oas.annotations.info.Info;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;


@SpringBootApplication
@OpenAPIDefinition(info = @Info(title = "Web Enabled Fingerprint Based EVM for Pvt. Communities", version = "1.0", description = "Candidate Management API"))
public class EvmServerApplication {

    public static void main(String[] args) {
        Environment.loadEnv();
        //start spring boot production application  java -Denv=production -jar evmserver.jar
        SpringApplication.run(EvmServerApplication.class, args);
        System.out.println("Application running...");
    }

    @Bean
    public CommandLineRunner testMongoConnection(MongoClient mongoClient) {
        return args -> {
            try {
                mongoClient.listDatabaseNames().first(); // ping
                System.out.println("✅ MongoDB connection successful!");
            } catch (Exception e) {
                System.err.println("❌ MongoDB connection failed: " + e.getMessage());
            }
        };
    }


}
