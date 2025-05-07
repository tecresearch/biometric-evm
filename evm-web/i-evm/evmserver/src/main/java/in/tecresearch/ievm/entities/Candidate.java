package in.tecresearch.ievm.entities;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
@Document(collection ="candidates")
@Schema(description = "Represents a candidate in the election")
public class Candidate {

    @Id
    @Schema(description = "Unique identifier of the candidate", example = "65a9d1f2e3b7f12a9c4d3e5a")
    private String id;

    @Schema(description = "Full name of the candidate", example = "John Doe", required = true)
    private String name;

    @Schema(description = "Political party affiliation", example = "Independent")
    private String party;

    @Schema(description = "Position candidate is running for", example = "President", required = true)
    private String position;

    @Schema(description = "Total votes received", example = "150")

    private int voteCount = 0;
}