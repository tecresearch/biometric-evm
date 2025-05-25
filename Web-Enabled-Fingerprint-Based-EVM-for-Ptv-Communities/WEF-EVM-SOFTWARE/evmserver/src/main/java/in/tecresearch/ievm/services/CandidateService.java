package in.tecresearch.ievm.services;

import in.tecresearch.ievm.entities.Candidate;

import java.util.List;

public interface CandidateService {
    Candidate createCandidate(Candidate candidate);
    Candidate updateCandidate(Candidate candidate);
    List<Candidate> getAllCandidates();
    Candidate getCandidateById(String id);
    void deleteCandidate(String id);
    Candidate incrementVoteCount(String candidateId);
    List<Candidate> getCandidatesByPosition(String position);
}
