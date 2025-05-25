package in.tecresearch.ievm.config;


import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.CorsRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

@Configuration
public class CorsConfig implements WebMvcConfigurer {

    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**")  // Apply to all endpoints
            .allowedOrigins("*")    // Allow all origins
            .allowedMethods("*")    // Allow all HTTP methods
            .allowedHeaders("*")    // Allow all headers
            .allowCredentials(false)
            .maxAge(3600);         // Cache preflight response for 1 hour
    }
}