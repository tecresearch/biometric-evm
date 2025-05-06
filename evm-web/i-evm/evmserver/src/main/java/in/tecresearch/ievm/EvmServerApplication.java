package in.tecresearch.ievm;


import in.tecresearch.ievm.environment.Environment;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;


@SpringBootApplication
public class EvmServerApplication {

    public static void main(String[] args) {
        Environment.loadEnv();
        //start spring boot production application  java -Denv=production -jar evmserver.jar
        SpringApplication.run(EvmServerApplication.class, args);
        System.out.println("Application running...");
    }


}
