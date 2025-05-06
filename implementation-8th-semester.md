#WEB ENABLED FINGERPRINT BASED
ELECTRONIC VOTING MACHINE FOR 
PRIVATE COMMUNITIES

## 1. Database Implementation

### ER Diagram
```
+-------------+       +---------------+       +-------------+
|    Admins   |       |   Candidates  |       |   Voters    |
+-------------+       +---------------+       +-------------+
| PK: admin_id|------>| PK: candidate_|       | PK: voter_id|
|    name     |       |      id       |       |    name     |
| fingerprint |       |    name       |       | fingerprint |
| created_at  |       |    party      |       | has_voted   |
+-------------+       | fingerprint   |       | created_at  |
                     | FK: created_by|       +-------------+
                     +---------------+             |
                           ^                      |
                           |                      |
                           |                      v
                           |                +-------------+
                           +----------------|    Votes    |
                                            +-------------+
                                            | PK: vote_id |
                                            | FK: voter_id|
                                            | FK: candidate|
                                            | vote_time   |
                                            +-------------+
```

### MySQL Database Setup

```sql
-- Create database
CREATE DATABASE online_voting_system;
USE online_voting_system;

-- Admins table
CREATE TABLE admins (
    admin_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    fingerprint_template LONGBLOB NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
+----------------------+--------------+------+-----+-------------------+-------------------+
| Field                | Type         | Null | Key | Default           | Extra             |
+----------------------+--------------+------+-----+-------------------+-------------------+
| admin_id             | int          | NO   | PRI | NULL              | auto_increment    |
| name                 | varchar(100) | NO   |     | NULL              |                   |
| fingerprint_template | longblob     | NO   |     | NULL              |                   |
| created_at           | timestamp    | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
+----------------------+--------------+------+-----+-------------------+-------------------+

-- Candidates table
CREATE TABLE candidates (
    candidate_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    party VARCHAR(100),
    fingerprint_template LONGBLOB NOT NULL,
    created_by INT,
    FOREIGN KEY (created_by) REFERENCES admins(admin_id) ON DELETE CASCADE
);

+----------------------+--------------+------+-----+---------+----------------+
| Field                | Type         | Null | Key | Default | Extra          |
+----------------------+--------------+------+-----+---------+----------------+
| candidate_id         | int          | NO   | PRI | NULL    | auto_increment |
| name                 | varchar(100) | NO   |     | NULL    |                |
| party                | varchar(100) | YES  |     | NULL    |                |
| fingerprint_template | longblob     | NO   | MUL | NULL    |                |
| created_by           | int          | YES  | MUL | NULL    |                |
+----------------------+--------------+------+-----+---------+----------------+

-- Voters table
CREATE TABLE voters (
    voter_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    fingerprint_template LONGBLOB NOT NULL,
    has_voted BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
+----------------------+--------------+------+-----+-------------------+-------------------+
| Field                | Type         | Null | Key | Default           | Extra             |
+----------------------+--------------+------+-----+-------------------+-------------------+
| voter_id             | int          | NO   | PRI | NULL              | auto_increment    |
| name                 | varchar(100) | NO   |     | NULL              |                   |
| fingerprint_template | longblob     | NO   | MUL | NULL              |                   |
| has_voted            | tinyint(1)   | YES  |     | 0                 |                   |
| created_at           | timestamp    | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
+----------------------+--------------+------+-----+-------------------+-------------------+

-- Votes table
CREATE TABLE votes (
    vote_id INT AUTO_INCREMENT PRIMARY KEY,
    voter_id INT NOT NULL,
    candidate_id INT NOT NULL,
    vote_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (voter_id) REFERENCES voters(voter_id) ON DELETE CASCADE,
    FOREIGN KEY (candidate_id) REFERENCES candidates(candidate_id) ON DELETE CASCADE
);

+--------------+-----------+------+-----+-------------------+-------------------+
| Field        | Type      | Null | Key | Default           | Extra             |
+--------------+-----------+------+-----+-------------------+-------------------+
| vote_id      | int       | NO   | PRI | NULL              | auto_increment    |
| voter_id     | int       | NO   | MUL | NULL              |                   |
| candidate_id | int       | NO   | MUL | NULL              |                   |
| vote_time    | timestamp | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
+--------------+-----------+------+-----+-------------------+-------------------+

-- Create indexes for better performance
CREATE INDEX idx_voters_fingerprint ON voters(fingerprint_template(255));
CREATE INDEX idx_admins_fingerprint ON admins(fingerprint_template(255));
CREATE INDEX idx_candidates_fingerprint ON candidates(fingerprint_template(255));

##Describing Indexes

mysql> SHOW INDEX FROM admins;
+--------+------------+------------------------+--------------+----------------------+-----------+-------------+----------+--------+------+------------+---------+---------------+-------
| Table  | Non_unique | Key_name               | Seq_in_index | Column_name          | Collation | Cardinality | Sub_part | Packed | Null | Index_type | Comment | Index_comment | Visible | Expression |
+--------+------------+------------------------+--------------+----------------------+-----------+-------------+----------+--------+------+------------+---------+---------------+-------
| admins |          0 | PRIMARY                |            1 | admin_id             | A         |           0 |     NULL |   NULL |      | BTREE      |         |               | YES     | NULL       |
| admins |          1 | idx_admins_fingerprint |            1 | fingerprint_template | A         |           0 |      255 |   NULL |      | BTREE      |         |               | YES     | NULL       |
+--------+------------+------------------------+--------------+----------------------+-----------+-------------+----------+--------+------+------------+---------+---------------+-------

mysql> SHOW INDEX FROM candidates;
+------------+------------+----------------------------+--------------+----------------------+-----------+-------------+----------+--------+------+------------+---------+---------------+---------+------------+
| Table      | Non_unique | Key_name                   | Seq_in_index | Column_name          | Collation | Cardinality | Sub_part | Packed | Null | Index_type | Comment | Index_comment | Visible | Expression |
+------------+------------+----------------------------+--------------+----------------------+-----------+-------------+----------+--------+------+------------+---------+---------------+---------+------------+
| candidates |          0 | PRIMARY                    |            1 | candidate_id         | A         |           0 |     NULL |   NULL |      | BTREE      |         |               | YES     | NULL       |
| candidates |          1 | created_by                 |            1 | created_by           | A         |           0 |     NULL |   NULL | YES  | BTREE      |         |               | YES     | NULL       |
| candidates |          1 | idx_candidates_fingerprint |            1 | fingerprint_template | A         |           0 |      255 |   NULL |      | BTREE      |         |               | YES     | NULL       |
+------------+------------+----------------------------+--------------+----------------------+-----------+-------------+----------+--------+------+------------+---------+---------------
```
```
REST API Documentation
Base URL
https://portal.wef-evm.com/api

Authentication
Endpoint	Method	Description
/admin/admin-login	POST	Password-based admin login
/admin/admin-login-biometric	POST	Fingerprint-based admin login
Admin Endpoints
Endpoint	Method	Description
/admin/login-success-get-center-info	GET	Get voting center info
/admin/total-voters	GET	Get total registered voters
/admin/total-candidates	GET	Get total candidates
/admin/votes-cast	GET	Get total votes cast
/admin/add-candidate	POST	Add new candidate
/admin/add-voter	POST	Register new voter
/admin/candidate-lists	GET	List all candidates
/admin/analysis	GET	Get voting analytics
/admin/recent-activity	GET	Get recent system activity
/admin/evm-health	GET	Check system health status
Voter Endpoints
Endpoint	Method	Description
/voter/verify-fingerprint	POST	Verify voter fingerprint
/voter/cast-vote	POST	Submit vote
Candidate Endpoints
Endpoint	Method	Description
/candidate/list	GET	List all candidates
/candidate/{id}	GET	Get candidate details
 

```

---

## Notes
- **Typo**: Endpoint 9 is labeled `candidate-lists` in the image but may intend `candidate-lists`.
- **Authentication**: The system supports both password and biometric (fingerprint) login for admins.
### Project Structure
```
src/
├── main/
│   ├── java/com/onlinevotingsystem/
│   │   ├── config/
│   │   ├── controller/
│   │   ├── dto/
│   │   ├── exception/
│   │   ├── model/
│   │   ├── repository/
│   │   ├── security/
│   │   ├── service/
│   │   └── OnlineVotingSystemApplication.java
│   └── resources/
│       ├── application.properties
│       └── templates/
└── test/
```

### Dependencies (pom.xml)
```xml
<dependencies>
    <!-- Spring Boot Starter -->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-data-jpa</artifactId>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-security</artifactId>
    </dependency>
    
    <!-- Database -->
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <scope>runtime</scope>
    </dependency>
    
    <!-- Utilities -->
    <dependency>
        <groupId>org.apache.commons</groupId>
        <artifactId>commons-lang3</artifactId>
        <version>3.12.0</version>
    </dependency>
    <dependency>
        <groupId>io.jsonwebtoken</groupId>
        <artifactId>jjwt</artifactId>
        <version>0.9.1</version>
    </dependency>
    
    <!-- Testing -->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
    </dependency>
</dependencies>
```

### application.properties
```properties
# Database
spring.datasource.url=jdbc:mysql://localhost:3306/online_voting_system
spring.datasource.username=root
spring.datasource.password=yourpassword
spring.datasource.driver-class-name=com.mysql.cj.jdbc.Driver

# JPA
spring.jpa.hibernate.ddl-auto=update
spring.jpa.show-sql=true
spring.jpa.properties.hibernate.format_sql=true

# Server
server.port=8080

# JWT
jwt.secret=your-secret-key
jwt.expiration.ms=86400000 # 24 hours
```

### Model Classes

#### Admin.java
```java
package com.onlinevotingsystem.model;

import javax.persistence.*;
import java.util.Date;

@Entity
@Table(name = "admins")
public class Admin {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer adminId;
    
    @Column(nullable = false)
    private String name;
    
    @Lob
    @Column(nullable = false)
    private byte[] fingerprintTemplate;
    
    @Column(nullable = false, updatable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date createdAt = new Date();

    // Getters and Setters
    // Constructors
}
```

#### Candidate.java
```java
package com.onlinevotingsystem.model;

import javax.persistence.*;
import java.util.Date;

@Entity
@Table(name = "candidates")
public class Candidate {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer candidateId;
    
    @Column(nullable = false)
    private String name;
    
    private String party;
    
    @Lob
    @Column(nullable = false)
    private byte[] fingerprintTemplate;
    
    @ManyToOne
    @JoinColumn(name = "created_by", nullable = false)
    private Admin createdBy;
    
    @Column(updatable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date createdAt = new Date();

    // Getters and Setters
    // Constructors
}
```

#### Voter.java
```java
package com.onlinevotingsystem.model;

import javax.persistence.*;
import java.util.Date;

@Entity
@Table(name = "voters")
public class Voter {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer voterId;
    
    @Column(nullable = false)
    private String name;
    
    @Lob
    @Column(nullable = false)
    private byte[] fingerprintTemplate;
    
    @Column(nullable = false)
    private boolean hasVoted = false;
    
    @Column(nullable = false, updatable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date createdAt = new Date();

    // Getters and Setters
    // Constructors
}
```

#### Vote.java
```java
package com.onlinevotingsystem.model;

import javax.persistence.*;
import java.util.Date;

@Entity
@Table(name = "votes")
public class Vote {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer voteId;
    
    @ManyToOne
    @JoinColumn(name = "voter_id", nullable = false)
    private Voter voter;
    
    @ManyToOne
    @JoinColumn(name = "candidate_id", nullable = false)
    private Candidate candidate;
    
    @Column(nullable = false, updatable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date voteTime = new Date();

    // Getters and Setters
    // Constructors
}
```

### Repository Interfaces

#### AdminRepository.java
```java
package com.onlinevotingsystem.repository;

import com.onlinevotingsystem.model.Admin;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface AdminRepository extends JpaRepository<Admin, Integer> {
    boolean existsByFingerprintTemplate(byte[] fingerprintTemplate);
    Admin findByFingerprintTemplate(byte[] fingerprintTemplate);
}
```

#### CandidateRepository.java
```java
package com.onlinevotingsystem.repository;

import com.onlinevotingsystem.model.Candidate;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface CandidateRepository extends JpaRepository<Candidate, Integer> {
    boolean existsByFingerprintTemplate(byte[] fingerprintTemplate);
    Candidate findByFingerprintTemplate(byte[] fingerprintTemplate);
}
```

#### VoterRepository.java
```java
package com.onlinevotingsystem.repository;

import com.onlinevotingsystem.model.Voter;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface VoterRepository extends JpaRepository<Voter, Integer> {
    boolean existsByFingerprintTemplate(byte[] fingerprintTemplate);
    Voter findByFingerprintTemplate(byte[] fingerprintTemplate);
    List<Voter> findByHasVoted(boolean hasVoted);
}
```

#### VoteRepository.java
```java
package com.onlinevotingsystem.repository;

import com.onlinevotingsystem.model.Vote;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface VoteRepository extends JpaRepository<Vote, Integer> {
    boolean existsByVoter(Voter voter);
    long countByCandidate(Candidate candidate);
}
```

### DTO Classes

#### AdminDTO.java
```java
package com.onlinevotingsystem.dto;

public class AdminDTO {
    private String name;
    private String fingerprintTemplate; // Base64 encoded
    
    // Getters and Setters
    // Constructors
}
```

#### CandidateDTO.java
```java
package com.onlinevotingsystem.dto;

public class CandidateDTO {
    private String name;
    private String party;
    private String fingerprintTemplate; // Base64 encoded
    
    // Getters and Setters
    // Constructors
}
```

#### VoterDTO.java
```java
package com.onlinevotingsystem.dto;

public class VoterDTO {
    private String name;
    private String fingerprintTemplate; // Base64 encoded
    
    // Getters and Setters
    // Constructors
}
```

#### VoteDTO.java
```java
package com.onlinevotingsystem.dto;

public class VoteDTO {
    private String fingerprintTemplate; // Base64 encoded
    private Integer candidateId;
    
    // Getters and Setters
    // Constructors
}
```

#### AuthResponse.java
```java
package com.onlinevotingsystem.dto;

public class AuthResponse {
    private String token;
    private String message;
    
    // Getters and Setters
    // Constructors
}
```

### Security Configuration

#### JwtUtil.java
```java
package com.onlinevotingsystem.security;

import io.jsonwebtoken.*;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.core.Authentication;
import org.springframework.stereotype.Component;

import java.util.Date;

@Component
public class JwtUtil {
    @Value("${jwt.secret}")
    private String jwtSecret;
    
    @Value("${jwt.expiration.ms}")
    private int jwtExpirationMs;
    
    public String generateJwtToken(Authentication authentication) {
        UserDetailsImpl userPrincipal = (UserDetailsImpl) authentication.getPrincipal();
        
        return Jwts.builder()
                .setSubject((userPrincipal.getUsername()))
                .setIssuedAt(new Date())
                .setExpiration(new Date((new Date()).getTime() + jwtExpirationMs))
                .signWith(SignatureAlgorithm.HS512, jwtSecret)
                .compact();
    }
    
    public String getUserNameFromJwtToken(String token) {
        return Jwts.parser().setSigningKey(jwtSecret).parseClaimsJws(token).getBody().getSubject();
    }
    
    public boolean validateJwtToken(String authToken) {
        try {
            Jwts.parser().setSigningKey(jwtSecret).parseClaimsJws(authToken);
            return true;
        } catch (SignatureException e) {
            // Log error
        } catch (MalformedJwtException e) {
            // Log error
        } catch (ExpiredJwtException e) {
            // Log error
        } catch (UnsupportedJwtException e) {
            // Log error
        } catch (IllegalArgumentException e) {
            // Log error
        }
        return false;
    }
}
```

#### UserDetailsImpl.java
```java
package com.onlinevotingsystem.security;

import com.onlinevotingsystem.model.Admin;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;

import java.util.Collection;
import java.util.Collections;

public class UserDetailsImpl implements UserDetails {
    private Admin admin;
    
    public UserDetailsImpl(Admin admin) {
        this.admin = admin;
    }
    
    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {
        return Collections.singletonList(new SimpleGrantedAuthority("ROLE_ADMIN"));
    }
    
    @Override
    public String getPassword() {
        return null; // Using fingerprint only, no password
    }
    
    @Override
    public String getUsername() {
        return admin.getName();
    }
    
    @Override
    public boolean isAccountNonExpired() {
        return true;
    }
    
    @Override
    public boolean isAccountNonLocked() {
        return true;
    }
    
    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }
    
    @Override
    public boolean isEnabled() {
        return true;
    }
}
```

#### UserDetailsServiceImpl.java
```java
package com.onlinevotingsystem.security;

import com.onlinevotingsystem.model.Admin;
import com.onlinevotingsystem.repository.AdminRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
public class UserDetailsServiceImpl implements UserDetailsService {
    @Autowired
    AdminRepository adminRepository;
    
    @Override
    @Transactional
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        Admin admin = adminRepository.findByName(username)
                .orElseThrow(() -> new UsernameNotFoundException("Admin Not Found with username: " + username));
        
        return UserDetailsImpl.build(admin);
    }
    
    @Transactional
    public UserDetails loadUserByFingerprint(byte[] fingerprintTemplate) {
        Admin admin = adminRepository.findByFingerprintTemplate(fingerprintTemplate)
                .orElseThrow(() -> new UsernameNotFoundException("Admin Not Found with this fingerprint"));
        
        return new UserDetailsImpl(admin);
    }
}
```

#### AuthTokenFilter.java
```java
package com.onlinevotingsystem.security;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.web.authentication.WebAuthenticationDetailsSource;
import org.springframework.util.StringUtils;
import org.springframework.web.filter.OncePerRequestFilter;

import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

public class AuthTokenFilter extends OncePerRequestFilter {
    @Autowired
    private JwtUtil jwtUtil;
    
    @Autowired
    private UserDetailsServiceImpl userDetailsService;
    
    private static final Logger logger = LoggerFactory.getLogger(AuthTokenFilter.class);
    
    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {
        try {
            String jwt = parseJwt(request);
            if (jwt != null && jwtUtil.validateJwtToken(jwt)) {
                String username = jwtUtil.getUserNameFromJwtToken(jwt);
                
                UserDetails userDetails = userDetailsService.loadUserByUsername(username);
                UsernamePasswordAuthenticationToken authentication = new UsernamePasswordAuthenticationToken(
                        userDetails, null, userDetails.getAuthorities());
                authentication.setDetails(new WebAuthenticationDetailsSource().buildDetails(request));
                
                SecurityContextHolder.getContext().setAuthentication(authentication);
            }
        } catch (Exception e) {
            logger.error("Cannot set user authentication: {}", e);
        }
        
        filterChain.doFilter(request, response);
    }
    
    private String parseJwt(HttpServletRequest request) {
        String headerAuth = request.getHeader("Authorization");
        
        if (StringUtils.hasText(headerAuth) && headerAuth.startsWith("Bearer ")) {
            return headerAuth.substring(7);
        }
        
        return null;
    }
}
```

#### WebSecurityConfig.java
```java
package com.onlinevotingsystem.config;

import com.onlinevotingsystem.security.AuthTokenFilter;
import com.onlinevotingsystem.security.UserDetailsServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.method.configuration.EnableGlobalMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

@Configuration
@EnableWebSecurity
@EnableGlobalMethodSecurity(prePostEnabled = true)
public class WebSecurityConfig extends WebSecurityConfigurerAdapter {
    @Autowired
    UserDetailsServiceImpl userDetailsService;
    
    @Autowired
    private AuthTokenFilter authTokenFilter;
    
    @Override
    public void configure(AuthenticationManagerBuilder authenticationManagerBuilder) throws Exception {
        authenticationManagerBuilder.userDetailsService(userDetailsService).passwordEncoder(passwordEncoder());
    }
    
    @Bean
    @Override
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();
    }
    
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }
    
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http.cors().and().csrf().disable()
            .sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS).and()
            .authorizeRequests()
            .antMatchers("/api/auth/**").permitAll()
            .antMatchers("/api/admins/register").permitAll()
            .antMatchers("/api/test/**").permitAll()
            .anyRequest().authenticated();
        
        http.addFilterBefore(authTokenFilter, UsernamePasswordAuthenticationFilter.class);
    }
}
```

### Controller Classes

#### AuthController.java
```java
package com.onlinevotingsystem.controller;

import com.onlinevotingsystem.dto.AdminDTO;
import com.onlinevotingsystem.dto.AuthResponse;
import com.onlinevotingsystem.model.Admin;
import com.onlinevotingsystem.repository.AdminRepository;
import com.onlinevotingsystem.security.JwtUtil;
import com.onlinevotingsystem.security.UserDetailsImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.*;

import java.util.Base64;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/api/auth")
public class AuthController {
    @Autowired
    AuthenticationManager authenticationManager;
    
    @Autowired
    AdminRepository adminRepository;
    
    @Autowired
    PasswordEncoder encoder;
    
    @Autowired
    JwtUtil jwtUtil;
    
    @PostMapping("/admins/register")
    public ResponseEntity<?> registerAdmin(@RequestBody AdminDTO adminDTO) {
        if (adminRepository.existsByName(adminDTO.getName())) {
            return ResponseEntity.badRequest().body(new AuthResponse("Error: Username is already taken!"));
        }
        
        byte[] fingerprintTemplate = Base64.getDecoder().decode(adminDTO.getFingerprintTemplate());
        
        if (adminRepository.existsByFingerprintTemplate(fingerprintTemplate)) {
            return ResponseEntity.badRequest().body(new AuthResponse("Error: Fingerprint is already registered!"));
        }
        
        Admin admin = new Admin();
        admin.setName(adminDTO.getName());
        admin.setFingerprintTemplate(fingerprintTemplate);
        
        adminRepository.save(admin);
        
        return ResponseEntity.ok(new AuthResponse("Admin registered successfully!"));
    }
    
    @PostMapping("/admins/login")
    public ResponseEntity<?> authenticateAdmin(@RequestBody AdminDTO adminDTO) {
        byte[] fingerprintTemplate = Base64.getDecoder().decode(adminDTO.getFingerprintTemplate());
        
        Admin admin = adminRepository.findByFingerprintTemplate(fingerprintTemplate)
                .orElseThrow(() -> new RuntimeException("Error: Admin not found with this fingerprint"));
        
        Authentication authentication = authenticationManager.authenticate(
                new UsernamePasswordAuthenticationToken(admin.getName(), null));
        
        SecurityContextHolder.getContext().setAuthentication(authentication);
        String jwt = jwtUtil.generateJwtToken(authentication);
        
        return ResponseEntity.ok(new AuthResponse(jwt, "Admin authenticated successfully!"));
    }
}
```

#### AdminController.java
```java
package com.onlinevotingsystem.controller;

import com.onlinevotingsystem.dto.CandidateDTO;
import com.onlinevotingsystem.dto.VoterDTO;
import com.onlinevotingsystem.model.Candidate;
import com.onlinevotingsystem.model.Voter;
import com.onlinevotingsystem.repository.CandidateRepository;
import com.onlinevotingsystem.repository.VoterRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.*;

import java.util.Base64;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/api")
public class AdminController {
    @Autowired
    CandidateRepository candidateRepository;
    
    @Autowired
    VoterRepository voterRepository;
    
    @PostMapping("/candidates/register")
    @PreAuthorize("hasRole('ADMIN')")
    public ResponseEntity<?> registerCandidate(@RequestBody CandidateDTO candidateDTO) {
        byte[] fingerprintTemplate = Base64.getDecoder().decode(candidateDTO.getFingerprintTemplate());
        
        if (candidateRepository.existsByFingerprintTemplate(fingerprintTemplate)) {
            return ResponseEntity.badRequest().body("Error: Candidate fingerprint already registered!");
        }
        
        Candidate candidate = new Candidate();
        candidate.setName(candidateDTO.getName());
        candidate.setParty(candidateDTO.getParty());
        candidate.setFingerprintTemplate(fingerprintTemplate);
        
        // Set createdBy from current authenticated admin
        // This requires additional implementation to get current admin
        
        candidateRepository.save(candidate);
        
        return ResponseEntity.ok("Candidate registered successfully!");
    }
    
    @PostMapping("/voters/register")
    @PreAuthorize("hasRole('ADMIN')")
    public ResponseEntity<?> registerVoter(@RequestBody VoterDTO voterDTO) {
        byte[] fingerprintTemplate = Base64.getDecoder().decode(voterDTO.getFingerprintTemplate());
        
        if (voterRepository.existsByFingerprintTemplate(fingerprintTemplate)) {
            return ResponseEntity.badRequest().body("Error: Voter fingerprint already registered!");
        }
        
        Voter voter = new Voter();
        voter.setName(voterDTO.getName());
        voter.setFingerprintTemplate(fingerprintTemplate);
        
        voterRepository.save(voter);
        
        return ResponseEntity.ok("Voter registered successfully!");
    }
    
    @GetMapping("/votes/results")
    @PreAuthorize("hasRole('ADMIN')")
    public ResponseEntity<?> getVotingResults() {
        // Implement vote counting logic
        return ResponseEntity.ok("Voting results");
    }
}
```

#### VotingController.java
```java
package com.onlinevotingsystem.controller;

import com.onlinevotingsystem.dto.VoteDTO;
import com.onlinevotingsystem.model.*;
import com.onlinevotingsystem.repository.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Base64;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/api/votes")
public class VotingController {
    @Autowired
    VoterRepository voterRepository;
    
    @Autowired
    CandidateRepository candidateRepository;
    
    @Autowired
    VoteRepository voteRepository;
    
    @PostMapping("/cast")
    public ResponseEntity<?> castVote(@RequestBody VoteDTO voteDTO) {
        byte[] fingerprintTemplate = Base64.getDecoder().decode(voteDTO.getFingerprintTemplate());
        
        Voter voter = voterRepository.findByFingerprintTemplate(fingerprintTemplate)
                .orElseThrow(() -> new RuntimeException("Error: Voter not found with this fingerprint"));
        
        if (voter.isHasVoted()) {
            return ResponseEntity.badRequest().body("Error: This voter has already voted!");
        }
        
        Candidate candidate = candidateRepository.findById(voteDTO.getCandidateId())
                .orElseThrow(() -> new RuntimeException("Error: Candidate not found!"));
        
        Vote vote = new Vote();
        vote.setVoter(voter);
        vote.setCandidate(candidate);
        
        voteRepository.save(vote);
        
        // Mark voter as voted
        voter.setHasVoted(true);
        voterRepository.save(voter);
        
        return ResponseEntity.ok("Vote cast successfully!");
    }
    
    @GetMapping("/count")
    public ResponseEntity<?> countVotes() {
        // Implement vote counting logic
        return ResponseEntity.ok("Vote count");
    }
}
```

## 3. ESP32 Hardware Implementation

### Hardware Components
1. ESP32 Development Board
2. R307 Fingerprint Sensor
3. Breadboard and jumper wires
4. Power supply (USB or battery)

### Wiring Diagram
```
R307 Fingerprint Sensor <-> ESP32
VCC  -> 3.3V
GND  -> GND
TX   -> GPIO16 (RX2)
RX   -> GPIO17 (TX2)
```

### ESP32 Code (Arduino IDE)

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Server details
const char* serverUrl = "http://your-server-address:8080";

// Fingerprint sensor setup
HardwareSerial mySerial(2); // Using UART2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(115200);
  mySerial.begin(57600, SERIAL_8N1, 16, 17); // RX, TX
  
  // Initialize fingerprint sensor
  if (finger.begin(57600)) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  Serial.println("Ready to scan fingerprint...");
  int fingerprintID = getFingerprintID();
  delay(50); // Small delay
  
  if (fingerprintID != -1) {
    Serial.print("Detected fingerprint ID: ");
    Serial.println(fingerprintID);
    
    // Here you would implement different flows based on the mode
    // (admin login, voter registration, voting, etc.)
    // For example:
    if (isAdminMode()) {
      adminLogin(fingerprintID);
    } else {
      castVote(fingerprintID);
    }
  }
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;
  
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;
  
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  
  return finger.fingerID;
}

void adminLogin(int fingerprintID) {
  // Get the fingerprint template
  uint8_t templateBuffer[512];
  int templateLength = getFingerprintTemplate(templateBuffer);
  
  if (templateLength > 0) {
    // Convert template to Base64
    String base64Template = base64Encode(templateBuffer, templateLength);
    
    // Send to server
    String endpoint = "/api/auth/admins/login";
    String payload = "{\"fingerprint_template\":\"" + base64Template + "\"}";
    
    sendToServer(endpoint, payload);
  }
}

void castVote(int fingerprintID) {
  // Get the fingerprint template
  uint8_t templateBuffer[512];
  int templateLength = getFingerprintTemplate(templateBuffer);
  
  if (templateLength > 0) {
    // Convert template to Base64
    String base64Template = base64Encode(templateBuffer, templateLength);
    
    // Here you would get the selected candidate ID from user input
    int candidateId = 1; // This would come from user selection
    
    // Send to server
    String endpoint = "/api/votes/cast";
    String payload = "{\"fingerprint_template\":\"" + base64Template + "\",\"candidate_id\":" + candidateId + "}";
    
    sendToServer(endpoint, payload);
  }
}

int getFingerprintTemplate(uint8_t *templateBuffer) {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return 0;
  
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return 0;
  
  p = finger.getTemplate();
  if (p != FINGERPRINT_OK) return 0;
  
  memcpy(templateBuffer, finger.template, finger.templateLength);
  return finger.templateLength;
}

String base64Encode(uint8_t* data, size_t length) {
  const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String result;
  int i = 0;
  int j = 0;
  uint8_t charArray3[3];
  uint8_t charArray4[4];
  
  while (length--) {
    charArray3[i++] = *(data++);
    if (i == 3) {
      charArray4[0] = (charArray3[0] & 0xfc) >> 2;
      charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
      charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
      charArray4[3] = charArray3[2] & 0x3f;
      
      for (i = 0; i < 4; i++) {
        result += base64Chars[charArray4[i]];
      }
      i = 0;
    }
  }
  
  if (i) {
    for (j = i; j < 3; j++) {
      charArray3[j] = '\0';
    }
    
    charArray4[0] = (charArray3[0] & 0xfc) >> 2;
    charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
    charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
    charArray4[3] = charArray3[2] & 0x3f;
    
    for (j = 0; j < i + 1; j++) {
      result += base64Chars[charArray4[j]];
    }
    
    while (i++ < 3) {
      result += '=';
    }
  }
  
  return result;
}

void sendToServer(String endpoint, String payload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl + endpoint);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

bool isAdminMode() {
  // Implement logic to determine if in admin mode
  // Could be based on a physical switch or button press
  return false;
}
```

## 4. Security Considerations

### Data Encryption
1. **HTTPS**: Always use HTTPS for communication between ESP32 and the server
2. **JWT**: Use JWT tokens for authentication with short expiration times
3. **Fingerprint Template Protection**: Store templates securely in the database

### Template Storage
1
