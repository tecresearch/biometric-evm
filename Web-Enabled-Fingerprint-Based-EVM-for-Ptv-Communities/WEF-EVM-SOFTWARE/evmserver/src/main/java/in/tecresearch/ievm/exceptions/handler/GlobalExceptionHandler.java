package in.tecresearch.ievm.exceptions.handler;

import in.tecresearch.ievm.entities.Voter;
import in.tecresearch.ievm.exceptions.CandidateAlreadyExistsException;
import in.tecresearch.ievm.exceptions.CandidateNotFoundException;
import in.tecresearch.ievm.exceptions.DuplicateVoterException;
import in.tecresearch.ievm.exceptions.VoterNotFoundException;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;
import org.springframework.web.context.request.WebRequest;

import java.time.LocalDateTime;
import java.util.LinkedHashMap;
import java.util.Map;

@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(CandidateAlreadyExistsException.class)
    public ResponseEntity<Object> handleCandidateAlreadyExistsException(
            CandidateAlreadyExistsException ex, WebRequest request) {

        Map<String, Object> body = new LinkedHashMap<>();
        body.put("timestamp", LocalDateTime.now());
        body.put("status", HttpStatus.CONFLICT.value());
        body.put("error", HttpStatus.CONFLICT.getReasonPhrase());
        body.put("message", ex.getMessage());
        body.put("path", getRequestPath(request));

        return new ResponseEntity<>(body, HttpStatus.CONFLICT);
    }

    @ExceptionHandler(CandidateNotFoundException.class)
    public ResponseEntity<Object> handleCandidateNotFoundException(
            CandidateNotFoundException ex, WebRequest request) {

        Map<String, Object> body = new LinkedHashMap<>();
        body.put("timestamp", LocalDateTime.now());
        body.put("status", HttpStatus.NOT_FOUND.value());
        body.put("error", HttpStatus.NOT_FOUND.getReasonPhrase());
        body.put("message", ex.getMessage());
        body.put("path", getRequestPath(request));

        return new ResponseEntity<>(body, HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(DuplicateVoterException.class)
    public ResponseEntity<Object> handleDuplicateVoterException(
            DuplicateVoterException ex, WebRequest request) {

        Map<String, Object> body = new LinkedHashMap<>();
        body.put("timestamp", LocalDateTime.now());
        body.put("status", HttpStatus.CONFLICT.value());
        body.put("error", HttpStatus.CONFLICT.getReasonPhrase());
        body.put("message", ex.getMessage());
        body.put("path", getRequestPath(request));

        return new ResponseEntity<>(body, HttpStatus.CONFLICT);
    }

    @ExceptionHandler(VoterNotFoundException.class)
    public ResponseEntity<Object> handleVoterNotFoundException(
            VoterNotFoundException ex, WebRequest request) {

        Map<String, Object> body = new LinkedHashMap<>();
        body.put("timestamp", LocalDateTime.now());
        body.put("status", HttpStatus.NOT_FOUND.value());
        body.put("error", HttpStatus.NOT_FOUND.getReasonPhrase());
        body.put("message", ex.getMessage());
        body.put("path", getRequestPath(request));

        return new ResponseEntity<>(body, HttpStatus.NOT_FOUND);
    }

    // Utility method to extract clean request path
    private String getRequestPath(WebRequest request) {
        String description = request.getDescription(false);
        // Remove "uri=" prefix if present
        return description.startsWith("uri=") ? description.substring(4) : description;
    }
}