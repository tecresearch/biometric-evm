package in.tecresearch.ievm.controllers;

import in.tecresearch.ievm.entities.Candidate;
import in.tecresearch.ievm.services.CandidateService;
import io.swagger.v3.oas.annotations.Operation;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/api/candidates")
public class CandidateController {

    private final CandidateService candidateService;

    @Autowired
    public CandidateController(CandidateService candidateService) {
        this.candidateService = candidateService;
    }

    @PostMapping("/enroll")
    @Operation(
            summary = "Create a new candidate",
            description = "Registers a new candidate for the election"
          )
    public ResponseEntity<Candidate> createCandidate(@RequestBody Candidate candidate) {
        if(candidate.getVoteCount()>0){
            candidate.setVoteCount(0);
        }
        return ResponseEntity.ok(candidateService.createCandidate(candidate));
    }

    @PutMapping("/{id}")
    @Operation(
            summary = "Update a candidate",
            description = "Update a  candidate for the election"
    )
    public ResponseEntity<Candidate> updateCandidate(@PathVariable String id, @RequestBody Candidate candidate) {
        if(candidate.getVoteCount()>0){
            candidate.setVoteCount(0);
        }
        candidate.setId(id);
        return ResponseEntity.ok(candidateService.updateCandidate(candidate));
    }

    @GetMapping
    @Operation(
            summary = "Get all candidate",
            description = "Get all registered candidate for the election"
    )
    public ResponseEntity<List<Candidate>> getAllCandidates() {
        return ResponseEntity.ok(candidateService.getAllCandidates());
    }

    @GetMapping("/{id}")
    @Operation(
            summary = "Get candidate by id",
            description = "Get single registered candidate for the election"
    )
    public ResponseEntity<Candidate> getCandidateById(@PathVariable String id) {
        return ResponseEntity.ok(candidateService.getCandidateById(id));
    }

    @DeleteMapping("/{id}")
    @Operation(
            summary = "Delete candidate by id",
            description = "Delete single registered candidate for the election"
    )
    public ResponseEntity<Void> deleteCandidate(@PathVariable String id) {
        candidateService.deleteCandidate(id);
        return ResponseEntity.noContent().build();
    }

    @PostMapping("/{id}/vote")
    @Operation(
            summary = "Caste vote to candidate by id",
            description = "caste vote to registered candidate for the election"
    )
    public ResponseEntity<Candidate> incrementVoteCount(@PathVariable String id) {
        return ResponseEntity.ok(candidateService.incrementVoteCount(id));
    }

    @GetMapping("/position/{position}")
    @Operation(
            summary = "Get candidate by position",
            description = "Get single registered candidate for the election via position"
    )
    public ResponseEntity<List<Candidate>> getCandidatesByPosition(@PathVariable String position) {
        return ResponseEntity.ok(candidateService.getCandidatesByPosition(position));
    }
}