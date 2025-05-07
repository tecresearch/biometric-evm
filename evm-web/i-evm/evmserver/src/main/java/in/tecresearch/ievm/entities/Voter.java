package in.tecresearch.ievm.entities;

import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;
import io.swagger.v3.oas.annotations.media.Schema;

@Document(collection = "voters")
@Schema(description = "Represents a voter in the system")
public class Voter {

    @Id
    @Schema(description = "Auto-generated voter ID from ESP32", example = "VTR2025001", required = true)
    private String voterId;

    @Schema(description = "Fingerprint template data from ESP32", required = true)
    private String fingerprintData;

    @Schema(description = "Whether the voter has cast their vote", example = "false")
    private boolean hasVoted = false;

    // Constructors
    public Voter() {}

    public Voter(String voterId, String fingerprintData) {
        this.voterId = voterId;
        this.fingerprintData = fingerprintData;
    }

    // Getters and Setters
    public String getVoterId() {
        return voterId;
    }

    public void setVoterId(String voterId) {
        this.voterId = voterId;
    }

    public String getFingerprintData() {
        return fingerprintData;
    }

    public void setFingerprintData(String fingerprintData) {
        this.fingerprintData = fingerprintData;
    }

    public boolean isHasVoted() {
        return hasVoted;
    }

    public void setHasVoted(boolean hasVoted) {
        this.hasVoted = hasVoted;
    }
}