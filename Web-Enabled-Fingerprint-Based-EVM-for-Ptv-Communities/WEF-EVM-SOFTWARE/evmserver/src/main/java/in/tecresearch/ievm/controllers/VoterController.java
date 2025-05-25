package in.tecresearch.ievm.controllers;

import in.tecresearch.ievm.entities.Candidate;
import in.tecresearch.ievm.entities.Voter;
import in.tecresearch.ievm.services.VoterService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/voters")
@Tag(name = "Voter Management", description = "Endpoints for managing voters")
public class VoterController {

    private final VoterService voterService;

    public VoterController(VoterService voterService) {
        this.voterService = voterService;
    }

    @PostMapping("/enroll")
    @Operation(summary = "Register a new voter")
    public ResponseEntity<Voter> registerVoter(@RequestBody Voter voter) {
        Voter registeredVoter = voterService.registerVoter(voter);
        return ResponseEntity.status(HttpStatus.CREATED).body(registeredVoter);
    }

    @GetMapping
    @Operation(
            summary = "Get all voters",
            description = "Get all registered voters for the election"
    )
    public ResponseEntity<List<Voter>> getAllCandidates() {
        return ResponseEntity.ok(voterService.getAllVoter());
    }

    @GetMapping("/{voterId}")
    @Operation(summary = "Get voter by ID")
    public ResponseEntity<Voter> getVoterById(@PathVariable String voterId) {
        Optional<Voter> voter = voterService.getVoterById(voterId);
        return voter.map(ResponseEntity::ok)
                   .orElseGet(() -> ResponseEntity.notFound().build());
    }

    @GetMapping("/by-fingerprint/{fingerprintData}")
    @Operation(summary = "Get voter by fingerprint")
    public ResponseEntity<Voter> getVoterByFingerprint(@PathVariable String fingerprintData) {
        Optional<Voter> voter = voterService.getVoterByFingerprint(fingerprintData);
        return voter.map(ResponseEntity::ok)
                   .orElseGet(() -> ResponseEntity.notFound().build());
    }

    @GetMapping("/verify-fingerprint/{fingerprintData}")
    @Operation(summary = "Verify fingerprint")
    public ResponseEntity<Boolean> verifyFingerprint(@PathVariable String fingerprintData) {
        boolean isValid = voterService.verifyFingerprint(fingerprintData);
        return ResponseEntity.ok(isValid);
    }

    @GetMapping("/login/verify-voter/{voterId}/{fingerprintData}")
    @Operation(summary = "Verify voter ID and fingerprint match")
    public ResponseEntity<Boolean> verifyVoterAndFingerprint(
            @PathVariable String voterId,
            @PathVariable String fingerprintData) {
        boolean isValid = voterService.verifyVoterAndFingerprint(voterId, fingerprintData);
        return ResponseEntity.ok(isValid);
    }

    @PostMapping("/{voterId}/mark-voted")
    @Operation(summary = "Mark voter as having voted")
    public ResponseEntity<Void> markVoterAsVoted(@PathVariable String voterId) {
        voterService.markVoterAsVoted(voterId);
        return ResponseEntity.ok().build();
    }
}