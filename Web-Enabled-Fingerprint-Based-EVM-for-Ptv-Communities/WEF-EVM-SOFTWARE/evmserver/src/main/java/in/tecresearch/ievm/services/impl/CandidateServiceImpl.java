package in.tecresearch.ievm.services.impl;

import in.tecresearch.ievm.entities.Candidate;
import in.tecresearch.ievm.exceptions.CandidateAlreadyExistsException;
import in.tecresearch.ievm.exceptions.CandidateNotFoundException;
import in.tecresearch.ievm.repositories.CandidateRepository;
import in.tecresearch.ievm.services.CandidateService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class CandidateServiceImpl implements CandidateService {

    private final CandidateRepository candidateRepository;
    @Autowired
    public CandidateServiceImpl(CandidateRepository candidateRepository) {
        this.candidateRepository = candidateRepository;
    }

    @Override
    public Candidate createCandidate(Candidate candidate) {
        if (candidateRepository.existsById(candidate.getId())){
            throw new CandidateAlreadyExistsException("Candidate with this id already exists");
        }
        return candidateRepository.save(candidate);
    }

    @Override
    public Candidate updateCandidate(Candidate candidate) {
        if (!candidateRepository.existsById(candidate.getId())) {
            throw new CandidateNotFoundException("Candidate not found with id: " + candidate.getId());
        }
        return candidateRepository.save(candidate);
    }

    @Override
    public List<Candidate> getAllCandidates() {
        return candidateRepository.findAll();
    }

    @Override
    public Candidate getCandidateById(String id) {
        return candidateRepository.findById(id)
                .orElseThrow(() -> new CandidateNotFoundException("Candidate not found with id: " + id));
    }

    @Override
    public void deleteCandidate(String id) {
        if (!candidateRepository.existsById(id)) {
            throw new CandidateNotFoundException("Candidate not found with id: " + id);
        }
        candidateRepository.deleteById(id);
    }

    @Override
    public Candidate incrementVoteCount(String candidateId) {
        Candidate candidate = getCandidateById(candidateId);
        candidate.setVoteCount(candidate.getVoteCount() + 1);
        return candidateRepository.save(candidate);
    }

    @Override
    public List<Candidate> getCandidatesByPosition(String position) {
        return candidateRepository.findByPosition(position);
    }
}
